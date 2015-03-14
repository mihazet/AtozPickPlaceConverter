#include "project.h"
#include "utils.h"

#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>
#include <wx/textfile.h>

#include <algorithm>

wxDEFINE_EVENT(wxEVT_PROJECT_LOADED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_PROJECT_UPDATED, wxCommandEvent);

wxArrayString G_array_on_subpcb;
wxArrayString G_array_global;

// ----------------------------------------------------------------------------
// FileTemplate
// ----------------------------------------------------------------------------

FileTemplate::FileTemplate()
{
	m_template.push_back( ItemTemplate("pcad", ";", "refdes:pattern:name:value:layer:location_x:location_y:angle", false) );
	m_template.push_back( ItemTemplate("altum", ", ", "refdes:pattern:::location_x:location_y:::layer:angle:value", true) );
}

ItemTemplate *FileTemplate::FindByType(wxString type)
{
	for (size_t i = 0; i < m_template.size(); i++)
		if (m_template[i].type == type)
			return &m_template[i];
	return NULL;
}

// ----------------------------------------------------------------------------
// Project
// ----------------------------------------------------------------------------

Project::Project()
	: m_height(1.6), m_angle(0), m_size_x(0), m_size_y(0), m_offset_x(0), m_offset_y(0)
	, m_apply_offset(false), m_filename("")
{
	G_array_on_subpcb.Add("Unknown");
	G_array_on_subpcb.Add("Not use");
	G_array_on_subpcb.Add("Mark 1");
	G_array_on_subpcb.Add("Mark 2");
	G_array_on_subpcb.Add("Mark 3");
	G_array_on_subpcb.Add("Local");

	G_array_global.Add("Unknown");
	G_array_global.Add("Not use");
	G_array_global.Add("Mark 1");
	G_array_global.Add("Mark 2");
	G_array_global.Add("Mark 3");


	wxFileName configFileName = wxFileConfig::GetLocalFile("", wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	if (!configFileName.DirExists())
		configFileName.Mkdir();

	m_settingsCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "settings", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	m_compPcadCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "components_pcad", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	m_compAltumCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "components_altium", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	m_patternsPcadCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "patterns_pcad", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	m_patternsAltumCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "patterns_altium", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
	m_projectsCfg = new wxFileConfig("PickPlaceConverter", "Antrax", "projects", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
}

Project::~Project()
{
	if (!m_filename.IsEmpty())
	{
		SaveProjectInfo();
		SaveTypes();
		SavePatterns();
	}


	delete m_settingsCfg;
	delete m_compPcadCfg;
	delete m_compAltumCfg;
	delete m_patternsPcadCfg;
	delete m_patternsAltumCfg;
	delete m_projectsCfg;
}

void Project::Load(const wxString& filename, const wxString& type)
{
	ItemTemplate *templ = NULL;

	// save previous project
	if (!m_filename.IsEmpty())
	{
		SaveProjectInfo();
		SaveTypes();
		SavePatterns();
	}

//	wxString ext = wxFileName(filename).GetExt();

	if (type == "pcad")
	{
		templ = m_fileTempl.FindByType("pcad");
		m_activeCompsCfg = m_compPcadCfg;
		m_activePatternsCfg = m_patternsPcadCfg;
	}
	else if (type == "altum")
	{
		templ = m_fileTempl.FindByType("altum");
		m_activeCompsCfg = m_compAltumCfg;
		m_activePatternsCfg = m_patternsAltumCfg;
	}
	else
	{
		wxLogError("Don`t know how to open %s!", type);
		return ;
	}

	wxLogMessage("Opening %s", filename);

	if (templ && Parse(filename, templ))
	{
		// init
		m_filename = filename;
		LoadProjectInfo(filename);
		UpdatePCBFullSize();
		UpdateComponentFullName();
		LoadTypes();
		LoadPatterns();
		UpdateComponents();


		Notify(wxEVT_PROJECT_LOADED);

		wxLogMessage("Done.");
	}
}



// Saved
//
#define FID_ARRAY_OFFSET	1

enum
{
	INDEX_TOP_COMP = 0,
	INDEX_TOP_FID,
	INDEX_BOT_COMP,
	INDEX_BOT_FID,
	COUNT_INDEX,
};


void Project::SaveProd(const wxString& filename)
{
	long full_size_x = (long)(m_size_x * 1000);
	long full_size_y = (long)(m_size_y * 1000);
	long height = (long)(m_height * 1000);
	long subpcbs = m_pcbs.size();

	wxXmlDocument doc;
	wxXmlNode *root_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Data");
	wxXmlNode *node, *tmp_node, *last_child_node;

	doc.SetRoot(root_node);
	node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Product");
	last_child_node = NULL;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "General");
	tmp_node->AddAttribute("Size",  wxString::Format("%ld,%ld,%ld", full_size_x, full_size_y, height));
	tmp_node->AddAttribute("ActiveSide", "Top");
	tmp_node->AddAttribute("PcbType", (subpcbs > 1) ? "Multiple" : "Single");
	tmp_node->AddAttribute("AutoSideDetect", "no");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	tmp_node = CreateProductSideDescr(LAYER_TOP_NAME);
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;
	tmp_node = CreateProductSideDescr(LAYER_BOT_NAME);
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	wxXmlNode *panels_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Panels");
	node->InsertChildAfter(panels_node, last_child_node);

	wxXmlNode *templates_node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Templates");

	size_t comp_count = m_comps.size();
	for (int subpcb_index = 0; subpcb_index < subpcbs; subpcb_index++)
	{
		wxXmlNode *panel_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Panel");
		panels_node->AddChild(panel_node);
		panel_node->AddAttribute("Ref", wxString::Format("%d", subpcb_index + 1));
		panel_node->AddAttribute("Template", m_pcbs[subpcb_index].subpcb_name);
		panel_node->AddAttribute("Position", wxString::Format("%ld,%ld",
								 (long)((m_pcbs[subpcb_index].offset_x - m_offset_x) * 1000),
								 (long)((m_pcbs[subpcb_index].offset_y - m_offset_y) * 1000)));
		panel_node->AddAttribute("Angle", "0");

		wxXmlNode *template_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Template");
		templates_node->AddChild(template_node);
		template_node->AddAttribute("Name", m_pcbs[subpcb_index].subpcb_name);
		template_node->AddAttribute("Size",	wxString::Format("%ld,%ld,%ld",
									(long)((m_pcbs[subpcb_index].size_x) * 1000),
									(long)((m_pcbs[subpcb_index].size_y) * 1000),
									height));

		XmlNodePtrs nodes[COUNT_INDEX];
		nodes[INDEX_TOP_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
		nodes[INDEX_TOP_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
		nodes[INDEX_TOP_FID].parent->AddAttribute("LastFidIsBadmark", "no");
		nodes[INDEX_BOT_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
		nodes[INDEX_BOT_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
		nodes[INDEX_BOT_FID].parent->AddAttribute("LastFidIsBadmark", "no");

		for (size_t index = 0; index < comp_count; index++)
		{
			int tmp_ind;
			if (!(m_comps[index].pnp_enabled && (m_comps[index].pnp_subpcb_index == subpcb_index)))
				continue;
			if (m_comps[index].layer == LAYER_TOP_NAME)
			{
				tmp_ind = INDEX_TOP_COMP;
			}
			else if (m_comps[index].layer == LAYER_BOT_NAME)
			{
				tmp_ind = INDEX_BOT_COMP;
			}
			else
			{
				continue;
			}
			if (m_comps[index].designator.StartsWith(FIDMARD_DES_PREF))
			{
				PrintFiducial(&nodes[tmp_ind + FID_ARRAY_OFFSET], m_comps[index]);
			}
			else
			{
				PrintComponent(&nodes[tmp_ind], m_comps[index]);
			}
			//wxLogVerbose(_T("Inserted %s to board %s"), m_comps[index].designator, m_project.pcbs[subpcb_index].subpcb_name);
		}

		tmp_node = new wxXmlNode(template_node, wxXML_ELEMENT_NODE, "Variants");
		tmp_node = new wxXmlNode(template_node, wxXML_ELEMENT_NODE, "BottomSide");
		tmp_node->AddAttribute("VariantsOnly", "no");
		tmp_node->AddChild(nodes[INDEX_BOT_FID].parent);
		tmp_node->AddChild(nodes[INDEX_BOT_COMP].parent);

		tmp_node = new wxXmlNode(template_node, wxXML_ELEMENT_NODE, "TopSide");
		tmp_node->AddAttribute("VariantsOnly", "no");
		tmp_node->AddChild(nodes[INDEX_TOP_FID].parent);
		tmp_node->AddChild(nodes[INDEX_TOP_COMP].parent);

	}

	if (doc.Save(filename))
	{
		wxLogMessage(_T("Saved to %s"), filename);
		wxFileName fn(filename);
		fn.SetExt("pcb");
		SaveDD500File(&doc, fn.GetFullPath());
	}
}

#define DD500_SECTION(NAME)					(wxString::Format("[%s]", NAME))
#define DD500_PARAM(NAME, VALUE)			(wxString::Format("%-28s: %s", NAME, VALUE))
#define DD500_FIDMARK(NAME, X, Y)			(wxString::Format("%-32s|%8.3f|%8.3f| ", NAME, X, Y))
#define DD500_COMP(DES, NAME, X, Y, ANG)	(wxString::Format("%-8s|%-32s|%8.3f|%8.3f|%7.2f|", DES, NAME, X, Y, ANG))
#define DD500_CALC_COORD(COMP, COORD)		(COMP.pnp_location_##COORD + (m_pcbs[COMP.pnp_subpcb_index].offset_##COORD - m_offset_##COORD))

void Project::SaveDD500File(wxXmlDocument *xmlDoc, wxString file)
{
	if (!xmlDoc)
		return;

	wxFileName filename(file);
	wxTextFile doc(file);
	if (doc.Exists())
	{
		doc.Open();
		doc.Clear();
	}
	else
	{
		doc.Create();
	}
	doc.AddLine(DD500_SECTION(wxT("FileID 1.00")));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("General"));
	doc.AddLine(DD500_PARAM("Pcb name", 			(".\\" + filename.GetFullName())));
	doc.AddLine(DD500_PARAM("Barcode",				""));
	doc.AddLine(DD500_PARAM("Pcb units",			"mm"));
	doc.AddLine(DD500_PARAM("Pcb length",			wxString::Format("%.3f", m_size_x)));
	doc.AddLine(DD500_PARAM("Pcb width",			wxString::Format("%.3f", m_size_y)));
	doc.AddLine(DD500_PARAM("Pcb thickness",		wxString::Format("%.3f", m_height)));
	doc.AddLine(DD500_PARAM("Active side",			"Top"));
	doc.AddLine(DD500_PARAM("Pcb type",				"Single"));
	doc.AddLine(DD500_PARAM("Pcb orientation",		wxString::Format("%d", m_angle)));
	doc.AddLine(DD500_PARAM("Badmark support",		"Yes"));
	doc.AddLine(DD500_PARAM("Component library",	".\\Complib.clb"));
	doc.AddLine(DD500_PARAM("Fiducial library",		".\\Fidlib.flb"));
	doc.AddLine(DD500_PARAM("Pcb comment",			""));
	doc.AddLine(wxEmptyString);

	PrintDD500SideDescr(doc, LAYER_TOP_NAME);
	PrintDD500SideDescr(doc, LAYER_BOT_NAME);

	doc.Write();
	doc.Close();
}




void Project::UpdateComponents()
{
	for (size_t i = 0; i < m_comps.size(); i++)
	{
		UpdateComponent(m_comps[i], i);
	}
}

void Project::UpdatePCBFullSize()
{
	long subpcbs = m_pcbs.size();
	double x1_min = INFINITY, y1_min = INFINITY, x2_max = -INFINITY, y2_max = -INFINITY;

	for (long index = 0; index < subpcbs; index++)
	{
		if (m_pcbs[index].ref_point1_x < x1_min) x1_min = m_pcbs[index].ref_point1_x;
		if (m_pcbs[index].ref_point1_y < y1_min) y1_min = m_pcbs[index].ref_point1_y;
		if (m_pcbs[index].ref_point2_x > x2_max) x2_max = m_pcbs[index].ref_point2_x;
		if (m_pcbs[index].ref_point2_y > y2_max) y2_max = m_pcbs[index].ref_point2_y;
	}
	m_size_x = x2_max - x1_min;
	m_size_y = y2_max - y1_min;
	m_offset_x = x1_min;
	m_offset_y = y1_min;
}


void Project::Notify(wxEventType type)
{
	wxCommandEvent event(type);
	wxPostEvent(this, event);
}

// ----------------------------------------------------------------------------
// Private sections
// ----------------------------------------------------------------------------

bool Project::Parse(const wxString& filename, ItemTemplate *templ)
{
	wxTextFile file(filename);
	wxString str;
	wxArrayString format = wxStringTokenize(templ->format, ":", wxTOKEN_RET_EMPTY);

	if (!file.Open(wxCSConv("CP1251")))
	{
		wxLogError("File %s can`t be opened!", filename);
		return false;
	}

	m_comps.clear();

	if (templ->skipHeader)
	{
		file.RemoveLine(0);
		file.RemoveLine(0);
	}

	wxLogMessage("Parsing...");

	int line = 0;
	int index = 0;
	for (str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine(), line++)
	{
		if (str.IsEmpty())
			continue;

		wxArrayString strCols = wxStringTokenize(str, templ->delimiter, wxTOKEN_STRTOK);
		if (strCols.GetCount() < format.GetCount())
		{
			wxLogWarning("line %d (cols: %d/%d): invalid format: %s", line, strCols.GetCount(), format.GetCount(), str);
			continue;
		}

		Component comp;

		bool add = true;

		for (size_t col = 0; col < format.GetCount(); col++)
		{
			wxString value = RemoveQuotes(strCols[col]);

			if (format[col] == "refdes")
			{
				if (value.IsEmpty())
				{
					add = false;
					break;
				}
				comp.designator = value;
			}
			else if (format[col] == "pattern")
			{
				comp.cad_pattern = value;
			}
			else if (format[col] == "name")
			{
				comp.cad_name = value;
			}
			else if (format[col] == "value")
			{
				comp.cad_value = value;
			}
			else if (format[col] == "layer")
			{
				if (value.Upper().StartsWith("T"))
					comp.layer = LAYER_TOP_NAME;
				else
					comp.layer = LAYER_BOT_NAME;
			}
			else if (format[col] == "location_x")
			{
				if (value.Find("m") != wxNOT_FOUND)
					value.BeforeFirst('m').ToDouble(&comp.cad_location_x);
				else
					value.ToDouble(&comp.cad_location_x);
			}
			else if (format[col] == "location_y")
			{
				if (value.Find("m") != wxNOT_FOUND)
					value.BeforeFirst('m').ToDouble(&comp.cad_location_y);
				else
					value.ToDouble(&comp.cad_location_y);
			}
			else if (format[col] == "angle")
			{
				value.ToDouble(&comp.cad_angle);
				if (comp.cad_angle >= 360)
					comp.cad_angle -= 360;
			}
		}

		if (add)
		{
			comp.index = index;
		 	m_comps.push_back(comp);
			index++;
		}
	}

	return true;
}

void Project::UpdateComponent(Component& comp, size_t compIndex)
{
	ComponentTypeIt type_it = std::find_if(m_types.begin(), m_types.end(), TypeByName(comp.full_name));
	if (type_it == m_types.end())
		return;

	PatternIt pattern_it = std::find_if(m_patterns.begin(), m_patterns.end(), PatternByName(comp.pattern));
	if (pattern_it == m_patterns.end())
	{
		Pattern pattern;
		pattern.pattern = comp.pattern;
		LoadPattern(pattern);
		m_patterns.push_back(pattern);
		pattern_it = m_patterns.end() - 1;
	}

	if (type_it->override_name)
	{
		comp.pnp_name = type_it->pnp_name;
	}
	else
	{
		comp.pnp_name = type_it->value;
		if (pattern_it->add_pack_to_name)
			comp.pnp_name = comp.pnp_name.Prepend(pattern_it->pnp_package + " ");
	}
	comp.pnp_package = pattern_it->pnp_package;
	comp.pnp_footprint = pattern_it->pnp_footprint;


	double sin_alpha = sin(comp.cad_angle * (M_PI / 180));
	double cos_alpha = cos(comp.cad_angle * (M_PI / 180));
	comp.pnp_location_x = comp.cad_location_x + ( pattern_it->offset_x * cos_alpha + pattern_it->offset_y * sin_alpha);
	comp.pnp_location_y = comp.cad_location_y + (-pattern_it->offset_x * sin_alpha + pattern_it->offset_y * cos_alpha);
	comp.pnp_angle = comp.cad_angle + pattern_it->angle;
	if (comp.pnp_angle >= 360.0) comp.pnp_angle -= 360.0;

	if (m_apply_offset)
	{
		comp.pnp_location_x += m_offset_x;
		comp.pnp_location_y += m_offset_y;
	}

	int index;
	for (index = m_pcbs.size() - 1; index >= 0; index--)
	{
		if (IsInRoom(m_pcbs[index].ref_point1_x, m_pcbs[index].ref_point1_y,
					 m_pcbs[index].ref_point2_x, m_pcbs[index].ref_point2_y,
					 comp.pnp_location_x, comp.pnp_location_y)
		   )
		{
			break;
		}
	}


	if (index < 0)
	{
		wxLogWarning("Component %s (%s at %.3f,%.3f) outside from all PCBs!",
					 comp.designator, comp.full_name,
					 comp.pnp_location_x, comp.pnp_location_y);
		index = 0;
	}


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	comp.pnp_subpcb_index = index;
	comp.pnp_location_x -= m_pcbs[index].offset_x;
	comp.pnp_location_y -= m_pcbs[index].offset_y;
	comp.pnp_enabled = comp.enabled
							&& type_it->enabled
							&& pattern_it->enabled
							&& m_pcbs[index].enabled;
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	if (comp.designator.StartsWith(FIDMARD_DES_PREF))
	{
		int index_local  = 0;
		int index_global = 0;

		FidMarkIt fidmark_it = std::find_if(m_fidmarks.begin(), m_fidmarks.end(), FidMarkByName(comp.designator));
		if (fidmark_it == m_fidmarks.end())
		{
			FidMark fidmark;
			fidmark.designator = comp.designator;
			m_fidmarks.push_back(fidmark);
			fidmark_it = m_fidmarks.end() - 1;
		}


		fidmark_it->component_index = comp.index;
		for (int fm_index = m_fidmarks.size() - 1; fm_index >= 0; fm_index--)
		{
			//Component& tmp_fm_comp = m_comps[m_fidmarks[fm_index].component_index];
			ComponentIt tmp_fm_comp = std::find_if(m_comps.begin(), m_comps.end(), CompByIndex(m_fidmarks[fm_index].component_index));

			if (tmp_fm_comp->layer != comp.layer)
				continue;
			index_global |= 1 << (m_fidmarks[fm_index].usage_as_global);
			if (tmp_fm_comp->pnp_subpcb_index != comp.pnp_subpcb_index)
				continue;
			index_local |= 1 << (m_fidmarks[fm_index].usage_type);
		}

		if (FID_MARK_USE_UNKNOWN == fidmark_it->usage_type)
		{
			if (!(index_local & (1 << FID_MARK_USE_FM1)))
			{
				fidmark_it->usage_type = FID_MARK_USE_FM1;
			}
			else if (!(index_local & (1 << FID_MARK_USE_FM2)))
			{
				fidmark_it->usage_type = FID_MARK_USE_FM2;
			}
			else if (!(index_local & (1 << FID_MARK_USE_FM3)))
			{
				fidmark_it->usage_type = FID_MARK_USE_FM3;
			}
			else
			{
				fidmark_it->usage_type = FID_MARK_USE_IGNORE;
			}
		}

		if (FID_MARK_USE_UNKNOWN == fidmark_it->usage_as_global)
		{
			if (!(index_global & (1 << FID_MARK_USE_FM1)))
			{
				fidmark_it->usage_as_global = FID_MARK_USE_FM1;
			}
			else if (!(index_global & (1 << FID_MARK_USE_FM2)))
			{
				fidmark_it->usage_as_global = FID_MARK_USE_FM2;
			}
			else if (!(index_global & (1 << FID_MARK_USE_FM3)))
			{
				fidmark_it->usage_as_global = FID_MARK_USE_FM3;
			}
			else
			{
				fidmark_it->usage_as_global = FID_MARK_USE_IGNORE;
			}
		}
	}
}

void Project::UpdateComponentFullName()
{
	for (size_t i = 0; i < m_comps.size(); i++)
	{
		Component& component = m_comps[i];

		component.strip_value = component.cad_value;
		if (component.strip_value.StartsWith("DNP") || component.strip_value.EndsWith("DNP"))
		{
			component.strip_value.Replace("DNP", wxEmptyString);
			component.strip_value.Trim(true).Trim(false);
			component.enabled = false;
		}
		else
		{
			component.enabled = true;
		}

		if ((!component.strip_value.IsEmpty()) && component.strip_value != "{Value}" && component.strip_value != component.cad_name)
			component.full_name = component.cad_name + " " + component.strip_value;
		else
			component.full_name = component.cad_name;
		if ((!component.cad_pattern.IsEmpty()) && component.cad_pattern != component.cad_name)
			component.full_name.Prepend(component.cad_pattern + " ");
	}
}

void Project::LoadTypes()
{
	m_types.clear();

	for (size_t i = 0; i < m_comps.size(); i++)
	{
		ComponentTypeIt type_it = std::find_if(m_types.begin(), m_types.end(), TypeByName(m_comps[i].full_name));
		if (type_it == m_types.end())
		{
			ComponentType type;
			type.name = m_comps[i].full_name;
			LoadComponent(type, m_comps[i]);
			m_types.push_back(type);
			type_it = m_types.end() - 1;
		}
		else
		{
			type_it->comp_count++;
		}
		m_comps[i].pattern = type_it->pattern;
	}
}

void Project::SaveTypes()
{
	for (ComponentTypeIt it = m_types.begin(); it != m_types.end(); it++)
	{
		wxConfigPathChanger cfg_cd_to(m_activeCompsCfg, "/" + it->name + "/");
		m_activeCompsCfg->Write("pattern", it->pattern);
		m_activeCompsCfg->Write("pnp_name", it->pnp_name);
		m_activeCompsCfg->Write("override_name",  it->override_name);
		m_activeCompsCfg->Write("enabled", it->enabled);
	}
}

void Project::LoadPatterns()
{
	m_patterns.clear();

	for (size_t i = 0; i < m_comps.size(); i++)
	{
		PatternIt pattern_it = std::find_if(m_patterns.begin(), m_patterns.end(), PatternByName(m_comps[i].pattern));
		if (pattern_it == m_patterns.end())
		{
			Pattern pattern;
			pattern.pattern = m_comps[i].pattern;
			LoadPattern(pattern);
			m_patterns.push_back(pattern);
		}
		else
		{
			pattern_it->comp_count++;
		}
	}
}

void Project::SavePatterns()
{
	for (PatternIt it = m_patterns.begin(); it != m_patterns.end(); it++)
	{
		wxConfigPathChanger cfg_cd_to(m_activePatternsCfg, "/" + it->pattern + "/");
		m_activePatternsCfg->Write("pnp_package", it->pnp_package);
		m_activePatternsCfg->Write("add_pack_to_name", it->add_pack_to_name);
		m_activePatternsCfg->Write("pnp_footprint", it->pnp_footprint);
		m_activePatternsCfg->Write("offset_x", it->offset_x);
		m_activePatternsCfg->Write("offset_y", it->offset_y);
		m_activePatternsCfg->Write("angle", it->angle);
		m_activePatternsCfg->Write("enabled", it->enabled);
	}
}


// ----------------------------------------------------------------------------
// Save export stuff...
// ----------------------------------------------------------------------------

void Project::PrintDD500SideDescr(wxTextFile& doc, wxString side)
{
	int index_fm1 = -1;
	int index_fm2 = -1;
	int index_fm3 = -1;

	int comps_count = m_comps.size();
	doc.AddLine(DD500_SECTION(wxString::Format("%s side", (side == LAYER_BOT_NAME) ? "Bottom" : "Top")));
	doc.AddLine(DD500_PARAM("Use variants only",			"No"));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Fiducials"));
	doc.AddLine(DD500_PARAM("Use reference points",			"Yes"));
	doc.AddLine(DD500_PARAM("Check all panel refs",			"No"));
	doc.AddLine(DD500_PARAM("Check refs with both heads",	"No"));
	doc.AddLine(DD500_PARAM("Double-check refs",			"Yes"));
	doc.AddLine(DD500_PARAM("Check local refs",				"Yes"));
	doc.AddLine(DD500_PARAM("Use badmark fiducial",			"Yes"));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Global Reference Points"));
	doc.AddLine(";Fiducial name------------------|----X---|----Y---|-Comment------------");
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Reference Panel Reference Points"));
	doc.AddLine(";Fiducial name------------------|----X---|----Y---|-Comment------------");
	//USE GLOBAL FMs
	for (int index = m_fidmarks.size() - 1; index >= 0; index--)
	{
		int comp_index = m_fidmarks[index].component_index;
		//Component& comp = m_comps[comp_index];
		ComponentIt comp = std::find_if(m_comps.begin(), m_comps.end(), CompByIndex(comp_index));

		if (comp->layer.Upper() != side.Upper())
			continue;
		if (FID_MARK_USE_FM1 == m_fidmarks[index].usage_as_global)
		{
			index_fm1 = comp_index;
		}
		else if (FID_MARK_USE_FM2 == m_fidmarks[index].usage_as_global)
		{
			index_fm2 = comp_index;
		}
		else if (FID_MARK_USE_FM3 == m_fidmarks[index].usage_as_global)
		{
			index_fm3 = comp_index;
		}
	}
	if (index_fm1 >= 0)
	{
		Component& comp = m_comps[index_fm1];
		doc.AddLine(DD500_FIDMARK(comp.pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	if (index_fm2 >= 0)
	{
		Component& comp = m_comps[index_fm2];
		doc.AddLine(DD500_FIDMARK(comp.pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	if (index_fm3 >= 0)
	{
		Component& comp = m_comps[index_fm3];
		doc.AddLine(DD500_FIDMARK(comp.pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Panel Reference Points"));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Dispense"));
	doc.AddLine(DD500_PARAM("Move height",	"1.500"));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("Components"));
	doc.AddLine(";Ref----|-Component name-----------------|----X---|----Y---|-Angle-|Prio|-ID-");
	for (int index = 0; index < comps_count; index++)
	{
		Component& comp = m_comps[index];
		if ((!comp.pnp_enabled) || (comp.layer.Upper() != side.Upper()) || comp.designator.StartsWith(FIDMARD_DES_PREF))
			continue;
		doc.AddLine(DD500_COMP(comp.designator, comp.pnp_footprint, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y), comp.pnp_angle));
	}
	doc.AddLine(wxEmptyString);
}

void Project::PrintComponent(XmlNodePtrs *pNode, Component comp)
{
	wxString str;
	wxXmlNode *node;

	if (NULL == pNode)
		return;

	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Component");
	node->AddAttribute("Ref", comp.designator);
	node->AddAttribute("Name", comp.pnp_name);
	str = wxString::Format("%ld,%ld", (long)(comp.pnp_location_x * 1000), (long)(comp.pnp_location_y * 1000));
	node->AddAttribute("Position", str);
	str = wxString::Format("%d", (int)(comp.pnp_angle * 100));
	node->AddAttribute("Angle", str);

	pNode->parent->InsertChildAfter(node, pNode->last_child);
	pNode->last_child = node;
}

void Project::PrintFiducial(XmlNodePtrs *pNode, Component comp)
{
	wxString ref_str;
	wxXmlNode *node;

	if (NULL == pNode)
		return;

	FidMarkIt fmDescr_it = std::find_if(m_fidmarks.begin(), m_fidmarks.end(), FidMarkByName(comp.designator));

	if (fmDescr_it == m_fidmarks.end())
		return;

	if (FID_MARK_USE_FM1 == fmDescr_it->usage_type)
	{
		ref_str = "1";
	}
	else if (FID_MARK_USE_FM2 == fmDescr_it->usage_type)
	{
		ref_str = "2";
	}
	else if (FID_MARK_USE_FM3 == fmDescr_it->usage_type)
	{
		ref_str = "3";
	}
	else
	{
		return;
	}
	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducial");
	node->AddAttribute("Ref", ref_str);
	node->AddAttribute("CadRef", comp.designator);
	node->AddAttribute("Position", wxString::Format("%ld,%ld", (long)(comp.pnp_location_x * 1000), (long)(comp.pnp_location_y * 1000)));
	node->AddAttribute("Name", comp.pnp_name);

	pNode->parent->InsertChildAfter(node, pNode->last_child);
	pNode->last_child = node;
}

wxXmlNode *Project::CreateProductSideDescr(wxString side)
{
	wxString ref_str;
	wxXmlNode *side_node, *tmp_node;

	side_node = new wxXmlNode(wxXML_ELEMENT_NODE, (side == LAYER_BOT_NAME) ? "BottomSide" : "TopSide");
	side_node->AddAttribute("Barcode", "");

	wxXmlNode *prod_orientation_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Orientations");
	side_node->AddChild(prod_orientation_node);
	tmp_node = new wxXmlNode(prod_orientation_node, wxXML_ELEMENT_NODE, "PP050");
	tmp_node->AddAttribute("ProductOrientation", wxString::Format("%d", m_angle));

	wxXmlNode *prod_fiducials_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
	side_node->AddChild(prod_fiducials_node);
	prod_fiducials_node->AddAttribute("BadmarkSupport", "no");

	if (m_pcbs.size() > 1)
	{
		for (int index = m_fidmarks.size() - 1; index >= 0; index--)
		{
			FidMark& fm_descr = m_fidmarks[index];
			//Component& fm_comp = m_comps[fm_descr.component_index];
			ComponentIt fm_comp = std::find_if(m_comps.begin(), m_comps.end(), CompByIndex(fm_descr.component_index));

			if (fm_comp->layer.Upper() != side.Upper())
				continue;
			if (FID_MARK_USE_FM1 == fm_descr.usage_as_global)
			{
				ref_str = "1";
			}
			else if (FID_MARK_USE_FM2 == fm_descr.usage_as_global)
			{
				ref_str = "2";
			}
			else if (FID_MARK_USE_FM3 == fm_descr.usage_as_global)
			{
				ref_str = "3";
			}
			else
			{
				continue;
			}
			tmp_node = new wxXmlNode(prod_fiducials_node, wxXML_ELEMENT_NODE, "Fiducial");
			tmp_node->AddAttribute("Ref", ref_str);
			tmp_node->AddAttribute("CadRef", fm_comp->designator);
			tmp_node->AddAttribute("Position", wxString::Format("%ld,%ld", (long)(fm_comp->pnp_location_x * 1000), (long)(fm_comp->pnp_location_y * 1000)));
			tmp_node->AddAttribute("Name", fm_comp->pnp_name);
		}
	}
	tmp_node = new wxXmlNode(prod_fiducials_node, wxXML_ELEMENT_NODE, "PP050");
	tmp_node->AddAttribute("UseGlobalRefs", "yes");
	tmp_node->AddAttribute("CheckPanelRefs", (m_pcbs.size() > 1) ? "yes" : "no");
	tmp_node->AddAttribute("CheckLocalRefs", "yes");
	tmp_node->AddAttribute("HighAccuracy", "yes");
	return side_node;
}

// ----------------------------------------------------------------------------
// Load/Save project config
// ----------------------------------------------------------------------------

void Project::LoadProjectInfo(wxString filename)
{
	if (filename.IsEmpty())
		return;
	wxString filename_only = wxFileNameFromPath(filename);
	wxString filename_pref = ("Pick Place for ");
	wxString proj_name = wxFileName(filename).GetName();
	if (proj_name.StartsWith(filename_pref))
	{
		proj_name = proj_name.Mid(filename_pref.Len());
	}

	// поиск этого проекта в конфиге
	if (!m_projectsCfg->HasGroup(filename_only))
	{
		// если точно такого проекта нет,
		// ищем похожие ...
		wxConfigPathChanger cfg_cd_to(m_projectsCfg, "/");
		long index;
		wxString group;
		wxArrayString foundProjects;

		bool cont = m_projectsCfg->GetFirstGroup(group, index);
		while (cont)
		{

			// ищем совпадения в имени по первой половине
			int n = proj_name.Length() / 2;
			if (!wxStrncmp_String<wxString>(group, proj_name, n))
			{
				foundProjects.Add(group);
			}
			cont = m_projectsCfg->GetNextGroup(group, index);
		}

		if (foundProjects.GetCount() > 0)
		{
			wxString proj = wxGetSingleChoice("Select a project to copy info:", "Load info", foundProjects);
			if (!proj.IsEmpty())
				filename_only = proj;
		}
	}

	m_pcbs.clear();

	wxConfigPathChanger cfg_cd_to(m_projectsCfg, "/" + filename_only + "/");
	m_project_name = m_projectsCfg->Read("project_name", proj_name);
	m_height = m_projectsCfg->ReadDouble("pcb_height", m_height);
	m_angle = m_projectsCfg->ReadLong("pnp_angle", m_angle);
	m_apply_offset = m_projectsCfg->ReadBool("apply_offset", m_apply_offset);
	switch (m_angle)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			break;
		case 90:
			m_angle = 1;
			break;
		case 180:
			m_angle = 2;
			break;
		case 270:
			m_angle = 3;
			break;
		default:
			m_angle = 0;
			break;
	}
	//	long subpcbs = m_projectsCfg->ReadLong("subpcb_count", -1);
	//	if(subpcbs > 0)
	if (m_projectsCfg->HasGroup("subpcb"))
	{
		wxString subpcb_name;
		long dummy;
		wxConfigPathChanger cfg_cd_to_sub_pcb(m_projectsCfg, "subpcb/");
		for (bool have_group = m_projectsCfg->GetFirstGroup(subpcb_name, dummy);
				have_group;
				have_group = m_projectsCfg->GetNextGroup(subpcb_name, dummy)
			)
		{
			Subpcb new_subpcb;
			new_subpcb.subpcb_name = subpcb_name;
			new_subpcb.size_x   = m_projectsCfg->ReadDouble(subpcb_name + "/size_x", 100);
			new_subpcb.size_y   = m_projectsCfg->ReadDouble(subpcb_name + "/size_y", 100);
			new_subpcb.offset_x = m_projectsCfg->ReadDouble(subpcb_name + "/offset_x", 0);
			new_subpcb.offset_y = m_projectsCfg->ReadDouble(subpcb_name + "/offset_y", 0);
			new_subpcb.ref_point1_x = new_subpcb.offset_x;
			new_subpcb.ref_point1_y = new_subpcb.offset_y;
			new_subpcb.ref_point2_x = new_subpcb.offset_x + new_subpcb.size_x;
			new_subpcb.ref_point2_y = new_subpcb.offset_y + new_subpcb.size_y;
			new_subpcb.enabled = m_projectsCfg->ReadBool(subpcb_name + "enabled", 1);
			m_pcbs.push_back(new_subpcb);
		}
	}
	else
	{
		Subpcb new_subpcb;
		new_subpcb.subpcb_name = "default";
		new_subpcb.size_x = 100;
		new_subpcb.size_y = 100;
		new_subpcb.offset_x = 0;
		new_subpcb.offset_y = 0;
		new_subpcb.ref_point1_x = new_subpcb.offset_x;
		new_subpcb.ref_point1_y = new_subpcb.offset_y;
		new_subpcb.ref_point2_x = new_subpcb.offset_x + new_subpcb.size_x;
		new_subpcb.ref_point2_y = new_subpcb.offset_y + new_subpcb.size_y;
		new_subpcb.enabled = 1;
		m_pcbs.push_back(new_subpcb);
	}

	//Load fidmark info
	if (m_projectsCfg->HasGroup("fid_mark"))
	{
		wxString fid_mark_des;
		long dummy;
		wxConfigPathChanger cfg_cd_to_sub_pcb(m_projectsCfg, "fid_mark/");
		for (bool have_group = m_projectsCfg->GetFirstGroup(fid_mark_des, dummy);
				have_group;
				have_group = m_projectsCfg->GetNextGroup(fid_mark_des, dummy)
			)
		{
			wxString tmp_str;
			int str_index;
			FidMark fidmark;
			fidmark.designator = fid_mark_des;
			tmp_str = m_projectsCfg->Read(fid_mark_des + "/usage_type", G_array_on_subpcb[FID_MARK_USE_UNKNOWN]);
			str_index = G_array_on_subpcb.Index(tmp_str, false);
			fidmark.usage_type = (wxNOT_FOUND == str_index) ? FID_MARK_USE_UNKNOWN : str_index;
			tmp_str = m_projectsCfg->Read(fid_mark_des + "/usage_as_global", G_array_global[FID_MARK_USE_UNKNOWN]);
			str_index = G_array_global.Index(tmp_str, false);
			fidmark.usage_as_global = (wxNOT_FOUND == str_index) ? FID_MARK_USE_UNKNOWN : str_index;
			m_fidmarks.push_back(fidmark);
		}
	}
}


void Project::SaveProjectInfo()
{
	long subpcbs = m_pcbs.size();
	long fidmarks = m_fidmarks.size();
	wxConfigPathChanger cfg_cd_to(m_projectsCfg, "/" + wxFileNameFromPath(m_filename) + "/");
	m_projectsCfg->Write("project_name", m_project_name);
	m_projectsCfg->Write("pcb_height", m_height);
	m_projectsCfg->Write("pnp_angle", m_angle);
	m_projectsCfg->Write("apply_offset", m_apply_offset);
	//	m_projectsCfg->Write("subpcb_count", subpcbs);
	m_projectsCfg->DeleteGroup("subpcb");
	for (long index = 0; index < subpcbs; index++)
	{
		m_projectsCfg->Write("subpcb/" + m_pcbs[index].subpcb_name + "/size_x", m_pcbs[index].size_x);
		m_projectsCfg->Write("subpcb/" + m_pcbs[index].subpcb_name + "/size_y", m_pcbs[index].size_y);
		m_projectsCfg->Write("subpcb/" + m_pcbs[index].subpcb_name + "/offset_x", m_pcbs[index].offset_x);
		m_projectsCfg->Write("subpcb/" + m_pcbs[index].subpcb_name + "/offset_y", m_pcbs[index].offset_y);
		m_projectsCfg->Write("subpcb/" + m_pcbs[index].subpcb_name + "/enabled", m_pcbs[index].enabled);
	}
	//Save fidmark info
	m_projectsCfg->DeleteGroup("fid_mark");
	for (long index = 0; index < fidmarks; index++)
	{
		m_projectsCfg->Write("fid_mark/" + m_fidmarks[index].designator + "/usage_type", G_array_on_subpcb[m_fidmarks[index].usage_type]);
		m_projectsCfg->Write("fid_mark/" + m_fidmarks[index].designator + "/usage_as_global", G_array_global[m_fidmarks[index].usage_as_global]);
	}
	m_projectsCfg->Flush();
}

void Project::LoadComponent(ComponentType& compType, const Component &comp)
{
	compType.pattern = comp.cad_pattern;
	compType.pnp_name = comp.full_name;
	if (comp.strip_value.IsEmpty())
	{
		compType.value = comp.cad_value;
	}
	else
	{
		compType.value = ParseNominal(comp.designator, comp.strip_value);
		if ((compType.value).IsEmpty())
		{
			wxString msg = wxString::Format("Can`t parse value for %s (%s). Value is %s.", compType.name, comp.designator, comp.strip_value);
			wxLogWarning(msg);
			compType.value = comp.strip_value;
		}
	}
	if (m_activeCompsCfg->HasGroup(compType.name))
	{
		compType.is_new = 0;
		wxConfigPathChanger cfg_cd_to(m_activeCompsCfg, "/" + compType.name + "/");
		compType.pattern = m_activeCompsCfg->Read("pattern", compType.pattern);
		compType.pnp_name = m_activeCompsCfg->Read("pnp_name", compType.pnp_name);
		compType.override_name = m_activeCompsCfg->ReadBool("override_name", compType.override_name);
		compType.enabled = m_activeCompsCfg->ReadBool("enabled", compType.enabled);
	}
	else
	{
		compType.is_new = 1;
		// compType->enabled = comp.enabled;
		wxConfigPathChanger cfg_cd_to(m_activeCompsCfg, "/" + compType.name + "/");
		m_activeCompsCfg->Write("enabled", compType.enabled);
	}
}

void Project::LoadPattern(Pattern& pattern)
{
	wxString pattern_name = pattern.pattern;
	if (m_activePatternsCfg->HasGroup(pattern_name))
	{
		pattern.is_new = 0;
		wxConfigPathChanger cfg_cd_to(m_activePatternsCfg, "/" + pattern_name + "/");
		pattern.pnp_package = m_activePatternsCfg->Read("pnp_package", pattern_name);
		pattern.pnp_footprint = m_activePatternsCfg->Read("pnp_footprint", pattern_name);
		pattern.offset_x = m_activePatternsCfg->ReadDouble("offset_x", pattern.offset_x);
		pattern.offset_y = m_activePatternsCfg->ReadDouble("offset_y", pattern.offset_y);
		pattern.angle = m_activePatternsCfg->ReadDouble("angle", pattern.angle);
		pattern.add_pack_to_name = m_activePatternsCfg->ReadBool("add_pack_to_name", pattern.add_pack_to_name);
		pattern.enabled = m_activePatternsCfg->ReadBool("enabled", pattern.enabled);
	}
	else
	{
		pattern.is_new = 1;
		pattern.pnp_package = pattern_name;
		pattern.pnp_footprint = pattern_name;
		wxConfigPathChanger cfg_cd_to(m_activePatternsCfg, "/" + pattern.pattern + "/");
		m_activePatternsCfg->Write("enabled", pattern.enabled);
	}
}

bool Project::IsInRoom(double ref1_x, double ref1_y, double ref2_x, double ref2_y, double ptr_x, double ptr_y)
{
	double tmp;
	bool res;
	if (ref1_x > ref2_x)
	{
		tmp = ref2_x;
		ref2_x = ref1_x;
		ref1_x = tmp;
	}
	if (ref1_y > ref2_y)
	{
		tmp = ref2_y;
		ref2_y = ref1_y;
		ref1_y = tmp;
	}
	res   = (ptr_x >= ref1_x) && (ptr_x <= ref2_x) &&
			(ptr_y >= ref1_y) && (ptr_y <= ref2_y);
	return res;
}

