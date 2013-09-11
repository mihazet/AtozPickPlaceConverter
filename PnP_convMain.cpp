/***************************************************************
 * Name:      PnP_convMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Alatar ()
 * Created:   2012-12-16
 * Copyright: Alatar ()
 * License:
 **************************************************************/

#include "PnP_convMain.h"
#include <wx/msgdlg.h>
#include <wx/fileconf.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/xml/xml.h>

//(*InternalHeaders(PnP_convFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(tComponentDescr);
//WX_DEFINE_OBJARRAY(tComponentTypeDescr);
//WX_DEFINE_OBJARRAY(tPatternDescr);

int CmpCompTypeFunc(t_component_type_descr *a_arg1, t_component_type_descr *a_arg2)
{
   return (a_arg1->name).CmpNoCase(a_arg2->name);
}
int CmpPatternFunc(t_pattern_descr *a_arg1, t_pattern_descr *a_arg2)
{
   return (a_arg1->pattern).CmpNoCase(a_arg2->pattern);
}

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(PnP_convFrame)
const long PnP_convFrame::ID_TEXTCTRL1 = wxNewId();
const long PnP_convFrame::ID_PROP = wxNewId();
const long PnP_convFrame::ID_COMP_TABLE = wxNewId();
const long PnP_convFrame::ID_PANEL4 = wxNewId();
const long PnP_convFrame::ID_COMP_TYPE_TABLE = wxNewId();
const long PnP_convFrame::ID_PANEL1 = wxNewId();
const long PnP_convFrame::ID_PATTERNS_TABLE = wxNewId();
const long PnP_convFrame::ID_PANEL2 = wxNewId();
const long PnP_convFrame::ID_AUINOTEBOOK1 = wxNewId();
const long PnP_convFrame::ID_OPEN = wxNewId();
const long PnP_convFrame::ID_SAVE_PROD = wxNewId();
const long PnP_convFrame::ID_QUIT = wxNewId();
const long PnP_convFrame::idMenuAbout = wxNewId();
const long PnP_convFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PnP_convFrame,wxFrame)
    //(*EventTable(PnP_convFrame)
    //*)
END_EVENT_TABLE()

PnP_convFrame::PnP_convFrame(wxWindow* parent,wxWindowID id) :
	m_component_types_list(CmpCompTypeFunc), m_patterns_list(CmpPatternFunc)
{
    //(*Initialize(PnP_convFrame)
    wxMenuItem* mnuAbout;
    wxBoxSizer* BoxSizer3;
    wxMenu* Menu1;
    wxMenuItem* mnuQuit;
    wxBoxSizer* BoxSizer2;
    wxMenuItem* mnuOpen;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxMenuItem* mnuSaveProd;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1000,800));
    auiManager = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
    m_txtLog = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(-1,50), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    auiManager->AddPane(m_txtLog, wxAuiPaneInfo().Name(_T("auiPaneLog")).DefaultPane().Caption(_("Pane caption")).CaptionVisible(false).Row(1).Bottom().BestSize(wxSize(-1,50)));
    m_pgProps = new wxPropertyGrid(this,ID_PROP);
    auiManager->AddPane(m_pgProps, wxAuiPaneInfo().Name(_T("auiProperties")).DefaultPane().Caption(_("Pane caption")).CaptionVisible(false).Row(1).Right().BestSize(wxSize(100,-1)));
    auiMainNotebook = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT|wxAUI_NB_TAB_MOVE|wxAUI_NB_WINDOWLIST_BUTTON);
    Panel4 = new wxPanel(auiMainNotebook, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    m_comp_table = new cCompTable(&m_components_list, Panel4,ID_COMP_TABLE,wxDefaultPosition,wxDefaultSize);
    BoxSizer3->Add(m_comp_table, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel4->SetSizer(BoxSizer3);
    BoxSizer3->Fit(Panel4);
    BoxSizer3->SetSizeHints(Panel4);
    Panel1 = new wxPanel(auiMainNotebook, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    m_comp_type_table = new cCompTypeTable(&m_component_types_list, Panel1,ID_COMP_TYPE_TABLE,wxDefaultPosition,wxDefaultSize);
    BoxSizer1->Add(m_comp_type_table, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer1);
    BoxSizer1->Fit(Panel1);
    BoxSizer1->SetSizeHints(Panel1);
    Panel2 = new wxPanel(auiMainNotebook, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_pattern_table = new cPatternTable(&m_patterns_list, Panel2,ID_PATTERNS_TABLE,wxDefaultPosition,wxDefaultSize);
    BoxSizer2->Add(m_pattern_table, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel2);
    BoxSizer2->SetSizeHints(Panel2);
    auiMainNotebook->AddPage(Panel4, _("Componenets"));
    auiMainNotebook->AddPage(Panel1, _("Component Types"));
    auiMainNotebook->AddPage(Panel2, _("Packages"));
    auiManager->AddPane(auiMainNotebook, wxAuiPaneInfo().Name(_T("auiPaneGraphs")).CenterPane().Caption(_("Graphs view")).Floatable().Movable(false));
    auiManager->Update();
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    mnuOpen = new wxMenuItem(Menu1, ID_OPEN, _("Open PnP file\tCtrl+O"), _("Open file to export"), wxITEM_NORMAL);
    Menu1->Append(mnuOpen);
    mnuSaveProd = new wxMenuItem(Menu1, ID_SAVE_PROD, _("Save prod file\tCtrl+S"), _("Save data file for DIMA PP-050"), wxITEM_NORMAL);
    Menu1->Append(mnuSaveProd);
    mnuQuit = new wxMenuItem(Menu1, ID_QUIT, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(mnuQuit);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    mnuAbout = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(mnuAbout);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_OPEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PnP_convFrame::On_mnuOpenSelected);
    Connect(ID_SAVE_PROD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PnP_convFrame::On_mnuSaveProdSelected);
    Connect(ID_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PnP_convFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PnP_convFrame::OnAbout);
    //*)
	wxLog::SetActiveTarget(new wxLogTextCtrl(m_txtLog));
	wxLog::SetVerbose(true);
//	wxLog::SetVerbose(false);

	m_config = new wxFileConfig("PnP_conv", "Antrax");

}

PnP_convFrame::~PnP_convFrame()
{
	WX_CLEAR_ARRAY(m_component_types_list);
	WX_CLEAR_ARRAY(m_patterns_list);
	delete m_config;
	auiManager->UnInit();
    //(*Destroy(PnP_convFrame)
    //*)
}

void PnP_convFrame::OnQuit(wxCommandEvent& event)
{
	Close();
}

void PnP_convFrame::OnAbout(wxCommandEvent& event)
{
	wxString msg = wxbuildinfo(long_f);
	wxMessageBox(msg, _("Welcome to..."));
}

#define CSV_DELIMITER				';'
#define CFG_COMPONENT_SECTION			"component/"
#define CFG_PATTERN_SECTION			"package/"
enum {
	INDEX_TOP_COMP,
	INDEX_TOP_FID,
	INDEX_BOT_COMP,
	INDEX_BOT_FID,
	INDEX_COUNT
};

void PnP_convFrame::On_mnuOpenSelected(wxCommandEvent& event)
{
	wxString str, tmp_str;
	wxFileDialog dlg_open(this);
	t_component_type_descr *component_type;
	t_pattern_descr *comp_pattern;

	int tmp_index;
	double value;

	dlg_open.SetWindowStyleFlag(wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR|wxFD_PREVIEW);
	dlg_open.SetWildcard(_("PCAD PnP files (*.txt)|*.txt|All files (*)|*"));
	if(dlg_open.ShowModal() != wxID_OK)
		return;

#warning TODO (alatar#1#): Сделать диалог для выбора типа входного файла, пока только P-CAD

	wxTextFile file(dlg_open.GetPath());
	m_filename = dlg_open.GetPath();

	if(!file.Open())
		return;
//wxLogMessage(_T("File opened"));
	m_config->SetPath("/PCAD");
	m_components_list.Clear();
	WX_CLEAR_ARRAY(m_component_types_list);	m_component_types_list.Clear();
	WX_CLEAR_ARRAY(m_patterns_list);	m_patterns_list.Clear();
	for(str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine())
	{
		t_component_descr component;

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		component.designator = RemoveQuotes(tmp_str);

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		component.cad_pattern = RemoveQuotes(tmp_str);

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		component.cad_name = RemoveQuotes(tmp_str);

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		component.cad_value = RemoveQuotes(tmp_str);

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		component.layer = RemoveQuotes(tmp_str);

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		tmp_str = RemoveQuotes(tmp_str);
		if(tmp_str.ToDouble(&value))
			component.cad_location_x = value;
		else
			component.cad_location_x = 0;

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		tmp_str = RemoveQuotes(tmp_str);
		if(tmp_str.ToDouble(&value))
			component.cad_location_y = value;
		else
			component.cad_location_y = 0;

		tmp_str = str.BeforeFirst(CSV_DELIMITER); str = str.AfterFirst(CSV_DELIMITER);
		tmp_str = RemoveQuotes(tmp_str);
		if(tmp_str.ToDouble(&value))
			component.cad_angle = value;
		else
			component.cad_angle = 0;
wxLogMessage(_T("Found %s %s at %f %f %f"), component.designator, component.cad_name, component.cad_location_x, component.cad_location_y, component.cad_angle);

///Корректировка

		if(component.cad_value == "DNP")
			component.enabled = false;
		else
			component.enabled = true;

		if((!component.cad_value.IsEmpty()) && component.cad_value != "{Value}" && component.cad_value != component.cad_name)
			component.full_name = component.cad_name + " " + component.cad_value;
		else
			component.full_name = component.cad_name;
		if((!component.cad_pattern.IsEmpty()) && component.cad_pattern != component.cad_name)
			component.full_name.Prepend(component.cad_pattern + " ");

//Поиск настроек по компоненту
		component_type = new t_component_type_descr;
		component_type->name = component.full_name;
		tmp_index = m_component_types_list.Index(component_type);
		if(wxNOT_FOUND == tmp_index)
		{
			m_component_types_list.Add(component_type);
			if(m_config->HasGroup(CFG_COMPONENT_SECTION + component_type->name))
			{
				component_type->is_new = 0;
				wxConfigPathChanger cfg_cd_to(m_config, CFG_COMPONENT_SECTION + component_type->name+"/");
				component_type->pattern = m_config->Read("pattern", component.cad_pattern);
				component_type->pnp_name = m_config->Read("pnp_name", component.full_name);
				component_type->enabled = m_config->ReadBool("enabled", component.enabled);
			} else {
				component_type->is_new = 1;
				component_type->pattern = component.cad_pattern;
				component_type->pnp_name = component.full_name;
				component_type->enabled = component.enabled;
				wxConfigPathChanger cfg_cd_to(m_config, CFG_COMPONENT_SECTION + component_type->name+"/");
				m_config->Write("pattern", component_type->pattern);
				m_config->Write("pnp_name", component_type->pnp_name);
				m_config->Write("enabled", component_type->enabled);
			}
		} else {
			delete component_type;
			component_type = m_component_types_list[tmp_index];
			component_type->comp_count++;
		}
//Assign pattern
		component.pattern = component_type->pattern;
//Поиск настроек по корпусу
		comp_pattern = new t_pattern_descr;
		comp_pattern->pattern = component.pattern;
		tmp_index = m_patterns_list.Index(comp_pattern);
		if(wxNOT_FOUND == tmp_index)
		{
			m_patterns_list.Add(comp_pattern);
			if(m_config->HasGroup(CFG_PATTERN_SECTION + comp_pattern->pattern))
			{
				comp_pattern->is_new = 0;
				wxConfigPathChanger cfg_cd_to(m_config, CFG_PATTERN_SECTION + comp_pattern->pattern+"/");
				comp_pattern->pnp_package = m_config->Read("package", component.pattern);
				comp_pattern->pnp_footprint = m_config->Read("footprint", component.pattern);
				comp_pattern->offset_x = m_config->ReadDouble("offset_x", 0.0);
				comp_pattern->offset_y = m_config->ReadDouble("offset_y", 0.0);
				comp_pattern->angle = m_config->ReadDouble("angle", 0.0);
				comp_pattern->enabled = m_config->ReadBool("enabled", true);
			} else {
				comp_pattern->is_new = 1;
				comp_pattern->pnp_package = component.pattern;
				comp_pattern->pnp_footprint = component.pattern;
				wxConfigPathChanger cfg_cd_to(m_config, CFG_PATTERN_SECTION + comp_pattern->pattern+"/");
				m_config->Write("pnp_package", comp_pattern->pnp_package);
				m_config->Write("pnp_footprint", comp_pattern->pnp_footprint);
			}
		} else {
			delete comp_pattern;
			comp_pattern = m_patterns_list[tmp_index];
			comp_pattern->comp_count++;
		}



		m_components_list.Add(component);



	}
//	m_component_types_list.Sort(CmpCompTypeFunc);
	ReInitLists();
}

wxString PnP_convFrame::RemoveQuotes(const wxString a_str)
{
	wxString str = a_str;
	wxUniChar quote_char = a_str[0];

	if(((quote_char != '\'') && (quote_char != '"')) || (a_str[a_str.Len()-1] != quote_char))
	{
		return str.Trim(true).Trim(false);
	}
	str = a_str.Mid(1, a_str.Len()-2);
	str = str.Trim(true).Trim(false);
	return str;
}

void PnP_convFrame::UpdateComponents()
{
//Перенсчёт location и angle

}

void PnP_convFrame::PrintComponent(t_xml_node_ptrs *a_node, t_component_descr a_comp)
{
	wxString str;
	wxXmlNode *node;

	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Component");
	node->AddAttribute("Ref", a_comp.designator);
	node->AddAttribute("Name", a_comp.pnp_name);
	str = wxString::Format("%d,%d", (int)(a_comp.pnp_location_x*1000), (int)(a_comp.pnp_location_y*1000));
	node->AddAttribute("Position", str);
	str = wxString::Format("%d", (int)(a_comp.pnp_angle*100));
	node->AddAttribute("Angle", str);

	a_node->parent->InsertChildAfter(node, a_node->last_child);
	a_node->last_child = node;
	a_node->elemets_count ++;
}

void PnP_convFrame::PrintFiducial(t_xml_node_ptrs *a_node, t_component_descr a_comp)
{
	wxString str;
	wxXmlNode *node;

	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducial");
	str = wxString::Format("%d", a_node->elemets_count + 1);
	node->AddAttribute("Ref", str);
	node->AddAttribute("CadRef", a_comp.designator);
	str = wxString::Format("%d,%d", (int)(a_comp.pnp_location_x*1000), (int)(a_comp.pnp_location_y*1000));
	node->AddAttribute("Position", str);
	node->AddAttribute("Name", a_comp.pnp_name);

	a_node->parent->InsertChildAfter(node, a_node->last_child);
	a_node->last_child = node;
	a_node->elemets_count ++;
}

void PnP_convFrame::On_mnuSaveProdSelected(wxCommandEvent& event)
{
	wxFileDialog dlg_save(this, "Enter target filename",
			wxEmptyString, m_filename.BeforeLast('.') + ".prod",
			"PP-050 files (*.prod)|*.prod|DD-500 files (*.pcb)|*.pcb", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

//	dlg_save.SetWindowStyleFlag(wxFD_SAVE);
//	dlg_save.SetWildcard(_("PP-050 files (*.prod)|*.prod|DD-500 files (*.pcb)|*.pcb"));
//	dlg_save.SetFilename);
	if(dlg_save.ShowModal() != wxID_OK)
		return;

	wxXmlDocument doc; //все ноды рекурсивно удаляются в деструкторе документа
	wxXmlNode *root_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Data");
	wxXmlNode *node, *tmp_node, *last_child_node;
	t_xml_node_ptrs nodes[INDEX_COUNT];
	nodes[INDEX_TOP_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
	nodes[INDEX_TOP_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
	nodes[INDEX_TOP_FID].parent->AddAttribute("LastFidIsBadmark", "no");
	nodes[INDEX_BOT_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
	nodes[INDEX_BOT_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
	nodes[INDEX_BOT_FID].parent->AddAttribute("LastFidIsBadmark", "no");

// TODO (alatar#1#): Перенести в конструктор
	nodes[INDEX_TOP_COMP].last_child = NULL;	nodes[INDEX_TOP_COMP].elemets_count = 0;
	nodes[INDEX_TOP_FID].last_child = NULL;		nodes[INDEX_TOP_FID].elemets_count = 0;
	nodes[INDEX_BOT_COMP].last_child = NULL;	nodes[INDEX_BOT_COMP].elemets_count = 0;
	nodes[INDEX_BOT_FID].last_child = NULL;		nodes[INDEX_BOT_FID].elemets_count = 0;

	doc.SetRoot(root_node);
	node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Product"); last_child_node = NULL;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "General");
	tmp_node->AddAttribute("Size", "0,0,1600");
	tmp_node->AddAttribute("ActiveSide", "Top");
	tmp_node->AddAttribute("PcbType", "Single");
	tmp_node->AddAttribute("AutoSideDetect", "no");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "TopSide");
	tmp_node->AddAttribute("Barcode", "");
	new wxXmlNode(tmp_node, wxXML_ELEMENT_NODE, "Orientations");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "BottomSide");
	tmp_node->AddAttribute("Barcode", "");
	new wxXmlNode(tmp_node, wxXML_ELEMENT_NODE, "Orientations");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Panels");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;
	tmp_node = new wxXmlNode(tmp_node, wxXML_ELEMENT_NODE, "Panel");
	tmp_node->AddAttribute("Ref", "1");
	tmp_node->AddAttribute("Template", "default");
	tmp_node->AddAttribute("Position", "0,0");
	tmp_node->AddAttribute("Angle", "0");

//Список компонентов
	node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Templates");
	node = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Template");
	node->AddAttribute("Name", "default");
	node->AddAttribute("Size", "0,0,1600");
//	node->AddAttribute("OriginOffset", "0,0");

	size_t comp_count = m_components_list.Count();
wxLogMessage(_T("Fount %zu comps"), comp_count);
	for(size_t index = 0; index < comp_count; index++)
	{
		int tmp_ind;
		if(!m_components_list[index].enabled)
			continue;
wxLogMessage(_T("Inserted %s"), m_components_list[index].designator);
		if(m_components_list[index].layer.Upper() == "TOP")
		{
			tmp_ind = INDEX_TOP_COMP;
		} else if(m_components_list[index].layer.Upper() == "BOTTOM") {
			tmp_ind = INDEX_BOT_COMP;
		} else {
			continue;
		}
		if(m_components_list[index].designator.StartsWith("FM"))
		{
			PrintFiducial(&nodes[tmp_ind + 1], m_components_list[index]);
		} else {
			PrintComponent(&nodes[tmp_ind], m_components_list[index]);
		}
	}

	tmp_node = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Variants");
	tmp_node = new wxXmlNode(node, wxXML_ELEMENT_NODE, "BottomSide");
	tmp_node->AddAttribute("VariantsOnly", "no");
	tmp_node->AddChild(nodes[INDEX_BOT_FID].parent);
	tmp_node->AddChild(nodes[INDEX_BOT_COMP].parent);

	tmp_node = new wxXmlNode(node, wxXML_ELEMENT_NODE, "TopSide");
	tmp_node->AddAttribute("VariantsOnly", "no");
	tmp_node->AddChild(nodes[INDEX_TOP_FID].parent);
	tmp_node->AddChild(nodes[INDEX_TOP_COMP].parent);

	doc.Save(dlg_save.GetPath());
wxLogMessage(_T("Saved to %s"), dlg_save.GetPath());
}
