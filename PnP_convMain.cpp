/***************************************************************
 * Name:      PnP_convMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Alatar ()
 * Created:   2012-12-16
 * Copyright: Alatar ()
 * License:
 **************************************************************/

#include "PnP_convMain.h"
#include "comp_type_table.h"
#include "pattern_table.h"
#include "fid_mark_table.h"

#include <wx/msgdlg.h>
#include <wx/fileconf.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include <wx/regex.h>
#include <wx/numformatter.h>

//(*InternalHeaders(PnP_convFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

wxArrayString G_array_on_subpcb;
wxArrayString G_array_global;

#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(tComponentDescr);
WX_DEFINE_OBJARRAY(tSubPcbs);
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
int CmpFidMarkFunc(t_fid_mark_descr *a_arg1, t_fid_mark_descr *a_arg2)
{
	return (a_arg1->designator).CmpNoCase(a_arg2->designator);
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
const long PnP_convFrame::ID_PANEL_COMP = wxNewId();
const long PnP_convFrame::ID_GRID_COMP_TYPE = wxNewId();
const long PnP_convFrame::ID_PANEL_COMP_TYPE = wxNewId();
const long PnP_convFrame::ID_GRID_PATTERN = wxNewId();
const long PnP_convFrame::ID_PANEL_PAT = wxNewId();
const long PnP_convFrame::ID_GRID_FID_MARKS = wxNewId();
const long PnP_convFrame::ID_PANEL_FID = wxNewId();
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
    EVT_PG_CHANGED(ID_PROP, PnP_convFrame::OnPropertyGridChanged)
END_EVENT_TABLE()

PnP_convFrame::PnP_convFrame(wxWindow* parent,wxWindowID id) :
	m_component_types_list(CmpCompTypeFunc), m_patterns_list(CmpPatternFunc), m_fid_marks_list(CmpFidMarkFunc)
{
    //(*Initialize(PnP_convFrame)
    wxMenuItem* mnuAbout;
    wxBoxSizer* BoxSizerFid;
    wxPanel* PanelPat;
    wxPanel* PanelFid;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizerCompType;
    wxPanel* PanelComp;
    wxMenuItem* mnuQuit;
    wxMenuItem* mnuOpen;
    wxPanel* PanelCompType;
    wxMenuBar* MenuBar1;
    wxMenuItem* mnuSaveProd;
    wxMenu* Menu2;
    wxBoxSizer* BoxSizerPat;
    wxBoxSizer* BoxSizerComp;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1000,800));
    auiManager = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
    m_txtLog = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(-1,50), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    auiManager->AddPane(m_txtLog, wxAuiPaneInfo().Name(_T("auiPaneLog")).DefaultPane().Caption(_("Pane caption")).CaptionVisible(false).Row(1).Bottom().BestSize(wxSize(-1,50)));
    m_pgProps = new wxPropertyGrid(this,ID_PROP);
    auiManager->AddPane(m_pgProps, wxAuiPaneInfo().Name(_T("auiProperties")).DefaultPane().Caption(_("Pane caption")).CaptionVisible(false).Row(1).Right().BestSize(wxSize(200,-1)));
    auiMainNotebook = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT|wxAUI_NB_TAB_MOVE|wxAUI_NB_WINDOWLIST_BUTTON);
    PanelComp = new wxPanel(auiMainNotebook, ID_PANEL_COMP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_COMP"));
    BoxSizerComp = new wxBoxSizer(wxHORIZONTAL);
    m_comp_table = new cCompTable(&m_components_list, PanelComp,ID_COMP_TABLE,wxDefaultPosition,wxDefaultSize);
    BoxSizerComp->Add(m_comp_table, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelComp->SetSizer(BoxSizerComp);
    BoxSizerComp->Fit(PanelComp);
    BoxSizerComp->SetSizeHints(PanelComp);
    PanelCompType = new wxPanel(auiMainNotebook, ID_PANEL_COMP_TYPE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_COMP_TYPE"));
    BoxSizerCompType = new wxBoxSizer(wxHORIZONTAL);
    m_grd_comp_type = new wxGrid(PanelCompType, ID_GRID_COMP_TYPE, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID_COMP_TYPE"));
    BoxSizerCompType->Add(m_grd_comp_type, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelCompType->SetSizer(BoxSizerCompType);
    BoxSizerCompType->Fit(PanelCompType);
    BoxSizerCompType->SetSizeHints(PanelCompType);
    PanelPat = new wxPanel(auiMainNotebook, ID_PANEL_PAT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PAT"));
    BoxSizerPat = new wxBoxSizer(wxHORIZONTAL);
    m_grd_pattern = new wxGrid(PanelPat, ID_GRID_PATTERN, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID_PATTERN"));
    BoxSizerPat->Add(m_grd_pattern, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelPat->SetSizer(BoxSizerPat);
    BoxSizerPat->Fit(PanelPat);
    BoxSizerPat->SetSizeHints(PanelPat);
    PanelFid = new wxPanel(auiMainNotebook, ID_PANEL_FID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_FID"));
    BoxSizerFid = new wxBoxSizer(wxHORIZONTAL);
    m_grd_fid_mark = new wxGrid(PanelFid, ID_GRID_FID_MARKS, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID_FID_MARKS"));
    BoxSizerFid->Add(m_grd_fid_mark, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelFid->SetSizer(BoxSizerFid);
    BoxSizerFid->Fit(PanelFid);
    BoxSizerFid->SetSizeHints(PanelFid);
    auiMainNotebook->AddPage(PanelComp, _("Componenets"));
    auiMainNotebook->AddPage(PanelCompType, _("Component Types"));
    auiMainNotebook->AddPage(PanelPat, _("Patterns"));
    auiMainNotebook->AddPage(PanelFid, _("Fiducials"));
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

	wxFileConfig::GetLocalFile("test",wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR).Mkdir();
	m_cfg_settings =		new wxFileConfig("PnP_conv", "Antrax", "settings",		wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);
	m_cfg_components_pcad =		new wxFileConfig("PnP_conv", "Antrax", "components_pcad",	wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);
	m_cfg_components_altium =	new wxFileConfig("PnP_conv", "Antrax", "components_altium",	wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);
	m_cfg_patterns_pcad =		new wxFileConfig("PnP_conv", "Antrax", "patterns_pcad",		wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);
	m_cfg_patterns_altium =		new wxFileConfig("PnP_conv", "Antrax", "patterns_altium",	wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);
	m_cfg_projects =		new wxFileConfig("PnP_conv", "Antrax", "projects",		wxEmptyString,wxCONFIG_USE_LOCAL_FILE| wxCONFIG_USE_SUBDIR);

	m_component_types_table = new cCompTypeTable(&m_component_types_list);
	m_grd_comp_type->SetTable(m_component_types_table, false);
	m_pattern_table = new cPatternTable(&m_patterns_list);
	m_grd_pattern->SetTable(m_pattern_table, false);
	m_fid_marks_table = new cFidMarkTable(&m_components_list, &m_fid_marks_list, this);
	m_grd_fid_mark->SetTable(m_fid_marks_table, false);
	ReInitLists();
}

PnP_convFrame::~PnP_convFrame()
{
//	delete m_pattern_table;  // TODO (alatar#1#): Внимание! Утечка памяти!
	WX_CLEAR_ARRAY(m_component_types_list);
	WX_CLEAR_ARRAY(m_patterns_list);
	delete m_cfg_settings;
	delete m_cfg_components_pcad;
	delete m_cfg_components_altium;
	delete m_cfg_patterns_pcad;
	delete m_cfg_patterns_altium;
	delete m_cfg_projects;
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
	wxString		str, tmp_str;
	wxFileDialog		dlg_open(this);
	wxFileConfig		*cfg_components = NULL;
	wxFileConfig		*cfg_patterns = NULL;
	t_component_type_descr	*component_type;
	t_pattern_descr		*comp_pattern;

	int			tmp_index;
	double			value;

	dlg_open.SetWindowStyleFlag(wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR|wxFD_PREVIEW);
	dlg_open.SetWildcard(_("PCAD PnP files (*.txt)|*.txt|All files (*)|*"));
	if(dlg_open.ShowModal() != wxID_OK)
		return;

#warning TODO (alatar#1#): Сделать диалог для выбора типа входного файла, пока только P-CAD
	cfg_components = m_cfg_components_pcad;
	cfg_patterns = m_cfg_patterns_pcad;
	m_component_types_table->SetCompTypesConfig(m_cfg_components_pcad);
	m_pattern_table->SetPatternsConfig(m_cfg_patterns_pcad);

	wxTextFile file(dlg_open.GetPath());

	if(!file.Open())
	{
		wxLogError(_T("File can`t be opened!"));
		return;
	}
	m_components_list.Clear();
	WX_CLEAR_ARRAY(m_component_types_list);	m_component_types_list.Clear();
	WX_CLEAR_ARRAY(m_patterns_list);	m_patterns_list.Clear();
	WX_CLEAR_ARRAY(m_fid_marks_list);	m_fid_marks_list.Clear();

	m_project.apply_offset = true; //for PCAD
	m_project.fullfilename = dlg_open.GetPath();
	LoadProjectInfo(dlg_open.GetFilename());

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
//wxLogMessage(_T("Found %s %s at %f %f %f"), component.designator, component.cad_name, component.cad_location_x, component.cad_location_y, component.cad_angle);

///Корректировка
		component.strip_value = component.cad_value;
		if(component.strip_value.StartsWith("DNP") || component.strip_value.EndsWith("DNP"))
		{
			component.strip_value.Replace("DNP", wxEmptyString);
			component.strip_value.Trim(true).Trim(false);
			component.enabled = false;
		} else {
			component.enabled = true;
		}

		if((!component.strip_value.IsEmpty()) && component.strip_value != "{Value}" && component.strip_value != component.cad_name)
			component.full_name = component.cad_name + " " + component.strip_value;
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
			component_type->pattern = component.cad_pattern;
			component_type->pnp_name = component.full_name;
			ParseNominals(component_type, component.designator, component.strip_value);
			if(cfg_components->HasGroup(component_type->name))
			{
				component_type->is_new = 0;
				wxConfigPathChanger cfg_cd_to(cfg_components, "/"+component_type->name+"/");
				component_type->pattern = cfg_components->Read("pattern", component_type->pattern);
				component_type->pnp_name = cfg_components->Read("pnp_name", component_type->pnp_name);
				component_type->enabled = cfg_components->ReadBool("enabled", component.enabled);
				component_type->value = cfg_components->ReadDouble("value", component_type->value);
				component_type->unit = cfg_components->Read("unit", component_type->unit);
			} else {
				component_type->is_new = 1;
//				component_type->enabled = component.enabled;
				wxConfigPathChanger cfg_cd_to(cfg_components, "/"+component_type->name+"/");
				cfg_components->Write("enabled", component_type->enabled);
//				cfg_components->Write("pattern", component_type->pattern);
//				cfg_components->Write("pnp_name", component_type->pnp_name);
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
			if(cfg_patterns->HasGroup(comp_pattern->pattern))
			{
				comp_pattern->is_new = 0;
				wxConfigPathChanger cfg_cd_to(cfg_patterns, "/"+comp_pattern->pattern+"/");
				comp_pattern->pnp_package = cfg_patterns->Read("pnp_package", component.pattern);
				comp_pattern->pnp_footprint = cfg_patterns->Read("pnp_footprint", component.pattern);
				comp_pattern->offset_x = cfg_patterns->ReadDouble("offset_x", 0.0);
				comp_pattern->offset_y = cfg_patterns->ReadDouble("offset_y", 0.0);
				comp_pattern->angle = cfg_patterns->ReadDouble("angle", 0.0);
				comp_pattern->enabled = cfg_patterns->ReadBool("enabled", true);
			} else {
				comp_pattern->is_new = 1;
				comp_pattern->pnp_package = component.pattern;
				comp_pattern->pnp_footprint = component.pattern;
				wxConfigPathChanger cfg_cd_to(cfg_patterns, "/"+comp_pattern->pattern+"/");
				cfg_patterns->Write("enabled", comp_pattern->enabled);
//				cfg_patterns->Write("pnp_package", comp_pattern->pnp_package);
//				cfg_patterns->Write("pnp_footprint", comp_pattern->pnp_footprint);
			}
		} else {
			delete comp_pattern;
			comp_pattern = m_patterns_list[tmp_index];
			comp_pattern->comp_count++;
		}

		m_components_list.Add(component);
	}
//	m_component_types_list.Sort(CmpCompTypeFunc);
	UpdateComponents();
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

bool PnP_convFrame::ParseNominals(t_component_type_descr *a_component_type, wxString a_designator, wxString a_value)
{
	char ch;
	double val_part;
	double factor = 1;
	wxString val, unit;

	if(NULL == a_component_type)
		return false;

	a_component_type->value_postfix = wxEmptyString;
	ch = a_designator.Upper()[0];
	switch(ch)
	{
	case 'R':
		a_component_type->unit = "";
		break;
	case 'C':
		a_component_type->unit = "F";
		factor = 1e-6;
		break;
	case 'L':
		a_component_type->unit = "H";
		break;
	default:
		a_component_type->value = -1;
		a_component_type->unit = wxEmptyString;
		return true;
	}
wxLogMessage(_T(">> Input is %s for %s, initial PnP name %s, pattern %s"), a_value, a_designator, a_component_type->pnp_name, a_component_type->pattern);

	wxRegEx re_format1("^([[:digit:]]+)([pnumkKMrRfFhH]{1,2})([[:digit:]]+)(.*)$"); //1p1F bla-bla-bla
	wxRegEx re_format2("^([[:digit:]]+[.,]?[[:digit:]]*)([pnumkKM]?)[fFhH]?(.*)$"); //1.1pF bla-bla-bla
	if(re_format1.Matches(a_value))
	{
		wxString tmp_str = re_format1.GetMatch(a_value, 1)+"."+re_format1.GetMatch(a_value, 3)+re_format1.GetMatch(a_value, 2)+re_format1.GetMatch(a_value, 4);
		a_value = tmp_str;
	}
	if(!re_format2.Matches(a_value))
	{
		a_component_type->value = -1;
	return false;
	}
	val = re_format2.GetMatch(a_value, 1); val.Replace(",", ".");
	unit = re_format2.GetMatch(a_value, 2);
	a_component_type->value_postfix = re_format2.GetMatch(a_value, 3);

//wxLogMessage(_T(">> Input is %s, Val is %s, Unit is %s, Postfix is %s"), a_value, val, unit, a_component_type->value_postfix);

	if(!val.ToCDouble(&val_part))
	{
		a_component_type->value = -1;
//wxLogMessage(_T("Convert fail! Val is %s, converted val is %f"), val, val_part);
		return false;
	}
	a_component_type->value = val_part;

	ch = unit[0];
	switch(ch)
	{
	case 'p':
		factor = 1e-12;
		break;
	case 'n':
		factor = 1e-9;
		break;
	case 'u':
		factor = 1e-6;
		break;
	case 'm':
		factor = 1e-3;
		break;
	case 'k':
	case 'K':	//кривое название, но встречается часто
		factor = 1e3;
		break;
	case 'M':
		factor = 1e6;
		break;
	}
//wxLogMessage(_T("Scale: Val is %f, factor is %f"), a_component_type->value, factor);
	a_component_type->value *= factor;
	NormalizeNominal(a_component_type);
	a_component_type->pnp_name = a_component_type->pattern + " " + wxNumberFormatter::ToString(a_component_type->value, 8, wxNumberFormatter::Style::Style_NoTrailingZeroes)/*.Replace(",", ".")*/ + a_component_type->unit + a_component_type->value_postfix;
wxLogMessage(_T("<< Result is %s"), a_component_type->pnp_name);
	return true;
}

bool PnP_convFrame::NormalizeNominal(t_component_type_descr *a_component_type)
{
	if((NULL == a_component_type) || (a_component_type->value < 0))
	{
//wxLogMessage(_T("PnP_convFrame::NormalizeNominal fail on input check (%p, %d)"), a_component_type, (a_component_type==NULL)?0:a_component_type->value);
		return false;
	}
	if(a_component_type->value == 0)
	{
//wxLogMessage(_T("PnP_convFrame::NormalizeNominal Zero on input: %f - %s"), a_component_type->value, a_component_type->unit);
		return true;
	}
//wxLogMessage(_T("PnP_convFrame::NormalizeNominal %f - %s"), a_component_type->value, a_component_type->unit);

	if(a_component_type->value < 1e-9)
	{
		a_component_type->value /= 1e-12;
		a_component_type->unit.Prepend("p");
//wxLogMessage(_T("Step 1e-9 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else if (a_component_type->value < 1e-6) {
		if("F" == a_component_type->unit)
		{
			if (a_component_type->value < 1e-8)
			{
				a_component_type->value /= 1e-12;
				a_component_type->unit.Prepend("p");
			} else {
				a_component_type->value /= 1e-6;
				a_component_type->unit.Prepend("u");
			}
		} else {
			a_component_type->value /= 1e-9;
			a_component_type->unit.Prepend("n");
		}
//wxLogMessage(_T("Step 1e-6 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else if (a_component_type->value < 1e-3) {
		a_component_type->value /= 1e-6;
		a_component_type->unit.Prepend("u");
//wxLogMessage(_T("Step 1e-3 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else if (a_component_type->value < 1) {
		if(("F" == a_component_type->unit) && (a_component_type->value < 1e-1))
		{
			a_component_type->value /= 1e-6;
			a_component_type->unit.Prepend("u");
		} else {
			a_component_type->value /= 1e-3;
			a_component_type->unit.Prepend("m");
		}
//wxLogMessage(_T("Step 1e-0 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else if (a_component_type->value < 1e3) {
//		a_component_type->value /= 1e0;
//		a_component_type->unit.Prepend("");
//wxLogMessage(_T("Step 1e3 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else if (a_component_type->value < 1e6) {
		a_component_type->value /= 1e3;
		a_component_type->unit.Prepend("k");
//wxLogMessage(_T("Step 1e6 -> %f - %s"), a_component_type->value, a_component_type->unit);
	} else {
		a_component_type->value /= 1e6;
		a_component_type->unit.Prepend("M");
//wxLogMessage(_T("Step 1e9 -> %f - %s"), a_component_type->value, a_component_type->unit);
	}
	return true;
}

void PnP_convFrame::UpdateComponents()
{
	bool need_save_proj = false;
	size_t comp_count = m_components_list.GetCount();
	for(size_t index = 0; index < comp_count; index++)
	{
		need_save_proj = UpdateComponent(&m_components_list[index], index) || need_save_proj;
	}
	ReInitLists();
	if(need_save_proj)
		SaveProjectInfo();
}

bool PnP_convFrame::UpdateComponent(t_component_descr *a_component, size_t a_comp_index)
{
	bool need_save_proj = false;
	t_component_type_descr	*component_type;
	t_pattern_descr		*comp_pattern;
	t_fid_mark_descr	*fid_mark;
	int index;
//надо заполнить следующие поля:
//pnp_name
//pnp_package
//pnp_footprint
//pnp_location_x
//pnp_location_y
//pnp_angle
	if(NULL == a_component)
		return false;

	component_type = new t_component_type_descr;
	component_type->name = a_component->full_name;
	index = m_component_types_list.Index(component_type);
	delete component_type;
	if(wxNOT_FOUND == index)
		return false;
	component_type = m_component_types_list[index];

	comp_pattern = new t_pattern_descr;
	comp_pattern->pattern = a_component->pattern;
	index = m_patterns_list.Index(comp_pattern);
	delete comp_pattern;
	if(wxNOT_FOUND == index)
		return false;
	comp_pattern = m_patterns_list[index];

	a_component->pnp_name = component_type->pnp_name;
	a_component->pnp_package = comp_pattern->pnp_package;
	a_component->pnp_footprint = comp_pattern->pnp_footprint;

//Пересчёт location и angle
	double sin_alpha = sin(a_component->cad_angle*(M_PI/180));
	double cos_alpha = cos(a_component->cad_angle*(M_PI/180));
	a_component->pnp_location_x = a_component->cad_location_x + ( comp_pattern->offset_x * cos_alpha + comp_pattern->offset_y * sin_alpha);
	a_component->pnp_location_y = a_component->cad_location_y + (-comp_pattern->offset_x * sin_alpha + comp_pattern->offset_y * cos_alpha);
	a_component->pnp_angle = a_component->cad_angle + comp_pattern->angle;
	if(a_component->pnp_angle >= 360.0) a_component->pnp_angle -= 360.0;

	if(m_project.apply_offset)
	{
		a_component->pnp_location_x += m_project.offset_x;
		a_component->pnp_location_y += m_project.offset_y;
	}
	for (index = m_project.pcbs.GetCount()-1; index >= 0; index--)
	{
		if(IsInRoom(m_project.pcbs[index].ref_point1_x, m_project.pcbs[index].ref_point1_y,
			    m_project.pcbs[index].ref_point2_x, m_project.pcbs[index].ref_point2_y,
			    a_component->pnp_location_x, a_component->pnp_location_y)
			)
		{
			break;
		}
	}
	if(index < 0)
	{
		wxLogWarning("Component %s (%s at %.3f,%.3f) outside from all PCBs!", a_component->designator, a_component->full_name, a_component->pnp_location_x, a_component->pnp_location_y);
		index = 0;
	}
	a_component->pnp_subpcb_index = index;
	a_component->pnp_location_x -= m_project.pcbs[index].offset_x;
	a_component->pnp_location_y -= m_project.pcbs[index].offset_y;
	a_component->pnp_enabled = a_component->enabled && component_type->enabled && comp_pattern->enabled && m_project.pcbs[index].enabled;

//Реперный знак?
	if(a_component->designator.StartsWith("FM"))
	{
		int index_local  = 0;
		int index_global = 0;

		fid_mark = new t_fid_mark_descr;
		fid_mark->designator = a_component->designator;
		long fm_descr_index = m_fid_marks_list.Index(fid_mark);
		if(wxNOT_FOUND == fm_descr_index)
		{
			fid_mark->designator = a_component->designator;
			m_fid_marks_list.Add(fid_mark);
		} else {
			delete fid_mark;
			fid_mark = m_fid_marks_list[fm_descr_index];
		}

		fid_mark->component_index = a_comp_index;
		for(int fm_index = m_fid_marks_list.GetCount()-1; fm_index >= 0; fm_index--)
		{
			t_component_descr *tmp_fm_comp = &m_components_list[m_fid_marks_list[fm_index]->component_index];
			if(tmp_fm_comp->layer != a_component->layer)
				continue;
			index_global |= 1<<(m_fid_marks_list[fm_index]->usage_as_global);
			if(tmp_fm_comp->pnp_subpcb_index != a_component->pnp_subpcb_index)
				continue;
			index_local |= 1<<(m_fid_marks_list[fm_index]->usage_type);
		}
		if(FID_MARK_USE_UNKNOWN == fid_mark->usage_type)
		{
			if(!(index_local & (1<<FID_MARK_USE_FM1)))
			{
				fid_mark->usage_type = FID_MARK_USE_FM1;
			} else if(!(index_local & (1<<FID_MARK_USE_FM2))) {
				fid_mark->usage_type = FID_MARK_USE_FM2;
			} else if(!(index_local & (1<<FID_MARK_USE_FM3))) {
				fid_mark->usage_type = FID_MARK_USE_FM3;
			} else {
				fid_mark->usage_type = FID_MARK_USE_IGNORE;
			}
			need_save_proj = true;
		}
		if(FID_MARK_USE_UNKNOWN == fid_mark->usage_as_global)
		{
			if(!(index_global & (1<<FID_MARK_USE_FM1)))
			{
				fid_mark->usage_as_global = FID_MARK_USE_FM1;
			} else if(!(index_global & (1<<FID_MARK_USE_FM2))) {
				fid_mark->usage_as_global = FID_MARK_USE_FM2;
			} else if(!(index_global & (1<<FID_MARK_USE_FM3))) {
				fid_mark->usage_as_global = FID_MARK_USE_FM3;
			} else {
				fid_mark->usage_as_global = FID_MARK_USE_IGNORE;
			}
			need_save_proj = true;
		}
	}
	return need_save_proj;
}

#define FID_ARRAY_OFFSET	1

void PnP_convFrame::On_mnuSaveProdSelected(wxCommandEvent& event)
{
	wxFileDialog dlg_save(this, "Enter target filename",
			wxEmptyString, m_project.fullfilename.BeforeLast('.') + ".prod",
			"PP-050 files (*.prod)|*.prod|DD-500 files (*.pcb)|*.pcb", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

//	dlg_save.SetWindowStyleFlag(wxFD_SAVE);
//	dlg_save.SetWildcard(_("PP-050 files (*.prod)|*.prod|DD-500 files (*.pcb)|*.pcb"));
//	dlg_save.SetFilename);
	if(dlg_save.ShowModal() != wxID_OK)
		return;

	long	full_size_x = (long)(m_project.size_x*1000),
		full_size_y = (long)(m_project.size_y*1000),
		height = (long)(m_project.height*1000);
	long subpcbs = m_project.pcbs.GetCount();

	wxXmlDocument doc; //все ноды рекурсивно удаляются в деструкторе документа
	wxXmlNode *root_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Data");
	wxXmlNode *node, *tmp_node, *last_child_node;

	doc.SetRoot(root_node);
	node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Product"); last_child_node = NULL;

	tmp_node = new wxXmlNode(wxXML_ELEMENT_NODE, "General");
	tmp_node->AddAttribute("Size",  wxString::Format("%ld,%ld,%ld", full_size_x, full_size_y, height));
	tmp_node->AddAttribute("ActiveSide", "Top");
	tmp_node->AddAttribute("PcbType", (subpcbs > 1)?"Multiple":"Single");
	tmp_node->AddAttribute("AutoSideDetect", "no");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	tmp_node = CreateProductSideDescr("TOP");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;
	tmp_node = CreateProductSideDescr("BOT");
	node->InsertChildAfter(tmp_node, last_child_node);
	last_child_node = tmp_node;

	wxXmlNode *panels_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Panels");
	node->InsertChildAfter(panels_node, last_child_node);

//Список компонентов
	wxXmlNode *templates_node = new wxXmlNode(root_node, wxXML_ELEMENT_NODE, "Templates");

	size_t comp_count = m_components_list.GetCount();
	for (size_t subpcb_index = 0; (long)subpcb_index < subpcbs; subpcb_index++)
	{
		wxXmlNode *panel_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Panel");
		panels_node->AddChild(panel_node);
		panel_node->AddAttribute("Ref", wxString::Format("%zu", subpcb_index+1));
		panel_node->AddAttribute("Template", m_project.pcbs[subpcb_index].subpcb_name);
		panel_node->AddAttribute("Position", wxString::Format("%ld,%ld", (long)((m_project.pcbs[subpcb_index].offset_x-m_project.offset_x)*1000),
										 (long)((m_project.pcbs[subpcb_index].offset_y-m_project.offset_y)*1000)));
		panel_node->AddAttribute("Angle", "0"); //относительный поворот пока не используется

		wxXmlNode *template_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Template");
		templates_node->AddChild(template_node);
		template_node->AddAttribute("Name", m_project.pcbs[subpcb_index].subpcb_name);
		template_node->AddAttribute("Size",  wxString::Format("%ld,%ld,%ld",
									(long)((m_project.pcbs[subpcb_index].size_x)*1000),
									(long)((m_project.pcbs[subpcb_index].size_y)*1000),
									height));

		t_xml_node_ptrs nodes[INDEX_COUNT];
		nodes[INDEX_TOP_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
		nodes[INDEX_TOP_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
		nodes[INDEX_TOP_FID].parent->AddAttribute("LastFidIsBadmark", "no");
		nodes[INDEX_BOT_COMP].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Components");
		nodes[INDEX_BOT_FID].parent = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
		nodes[INDEX_BOT_FID].parent->AddAttribute("LastFidIsBadmark", "no");

		for(size_t index = 0; index < comp_count; index++)
		{
			int tmp_ind;
			if(!(m_components_list[index].pnp_enabled && (m_components_list[index].pnp_subpcb_index == subpcb_index)))
				continue;
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
				PrintFiducial(&nodes[tmp_ind + FID_ARRAY_OFFSET], m_components_list[index]);
			} else {
				PrintComponent(&nodes[tmp_ind], m_components_list[index]);
			}
//wxLogVerbose(_T("Inserted %s to board %s"), m_components_list[index].designator, m_project.pcbs[subpcb_index].subpcb_name);
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


	doc.Save(dlg_save.GetPath());
wxLogMessage(_T("Saved to %s"), dlg_save.GetPath());
	SaveDD500File(&doc, dlg_save.GetPath().BeforeLast('.') + ".pcb");
}

#define DD500_SECTION(a_NAME)				(wxString::Format("[%s]", a_NAME))
#define DD500_PARAM(a_NAME, a_VALUE)			(wxString::Format("%-28s: %s", a_NAME, a_VALUE))
#define DD500_FIDMARK(a_NAME, a_X, a_Y)			(wxString::Format("%-32s|%8.3f|%8.3f| ", a_NAME, a_X, a_Y))
#define DD500_COMP(a_DES, a_NAME, a_X, a_Y, a_ANG)	(wxString::Format("%-8s|%-32s|%8.3f|%8.3f|%7.2f|", a_DES, a_NAME, a_X, a_Y, a_ANG))
#define DD500_CALC_COORD(a_COMP, a_COORD)		(a_COMP->pnp_location_##a_COORD + (m_project.pcbs[a_COMP->pnp_subpcb_index].offset_##a_COORD - m_project.offset_##a_COORD))

void PnP_convFrame::SaveDD500File(wxXmlDocument *a_doc, wxString a_file)
{
	if(NULL == a_doc)
		return;
	wxFileName filename(a_file);
	wxTextFile doc(a_file);
	if(doc.Exists())
	{
		doc.Open();
		doc.Clear();
	} else {
		doc.Create();
	}
	doc.AddLine(DD500_SECTION(wxT("FileID 1.00")));
	doc.AddLine(wxEmptyString);
	doc.AddLine(DD500_SECTION("General"));
	doc.AddLine(DD500_PARAM("Pcb name", (".\\" + filename.GetFullName())));
	doc.AddLine(DD500_PARAM("Barcode",		""));
	doc.AddLine(DD500_PARAM("Pcb units",		"mm"));
	doc.AddLine(DD500_PARAM("Pcb length",		wxString::Format("%.3f", m_project.size_x)));
	doc.AddLine(DD500_PARAM("Pcb width",		wxString::Format("%.3f", m_project.size_y)));
	doc.AddLine(DD500_PARAM("Pcb thickness",	wxString::Format("%.3f", m_project.height)));
	doc.AddLine(DD500_PARAM("Active side",		"Top"));
	doc.AddLine(DD500_PARAM("Pcb type",		"Single"));
	doc.AddLine(DD500_PARAM("Pcb orientation",	wxString::Format("%d", m_project.angle)));
	doc.AddLine(DD500_PARAM("Badmark support",	"Yes"));
	doc.AddLine(DD500_PARAM("Component library",	".\\Complib.clb"));
	doc.AddLine(DD500_PARAM("Fiducial library",	".\\Fidlib.flb"));
	doc.AddLine(DD500_PARAM("Pcb comment",		""));
	doc.AddLine(wxEmptyString);

	PrintDD500SideDescr(doc, "TOP");
	PrintDD500SideDescr(doc, "BOT");

	doc.Write();
	doc.Close();
}
void PnP_convFrame::PrintDD500SideDescr(wxTextFile &a_doc, wxString a_side)
{
	int index_fm1 = -1,
	    index_fm2 = -1,
	    index_fm3 = -1;
	t_component_descr *comp;
	int comps_count = m_components_list.GetCount();
	a_doc.AddLine(DD500_SECTION(wxString::Format("%s side", (a_side.Upper() == "BOT")?"Bottom":"Top")));
	a_doc.AddLine(DD500_PARAM("Use variants only",		"No"));
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Fiducials"));
	a_doc.AddLine(DD500_PARAM("Use reference points",	"Yes"));
	a_doc.AddLine(DD500_PARAM("Check all panel refs",	"No"));
	a_doc.AddLine(DD500_PARAM("Check refs with both heads",	"No"));
	a_doc.AddLine(DD500_PARAM("Double-check refs",		"Yes"));
	a_doc.AddLine(DD500_PARAM("Check local refs",		"Yes"));
	a_doc.AddLine(DD500_PARAM("Use badmark fiducial",	"Yes"));
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Global Reference Points"));
	a_doc.AddLine(";Fiducial name------------------|----X---|----Y---|-Comment------------");
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Reference Panel Reference Points"));
	a_doc.AddLine(";Fiducial name------------------|----X---|----Y---|-Comment------------");
//USE GLOBAL FMs
	for(int index = m_fid_marks_list.GetCount()-1; index >= 0; index--)
	{
		int comp_index = m_fid_marks_list[index]->component_index;
		comp = &m_components_list[comp_index];
		if(comp->layer.Upper() != a_side.Upper())
			continue;
		if(FID_MARK_USE_FM1 == m_fid_marks_list[index]->usage_as_global)
		{
			index_fm1 = comp_index;
		} else if(FID_MARK_USE_FM2 == m_fid_marks_list[index]->usage_as_global) {
			index_fm2 = comp_index;
		} else if(FID_MARK_USE_FM3 == m_fid_marks_list[index]->usage_as_global) {
			index_fm3 = comp_index;
		}
	}
	if(index_fm1 >= 0)
	{
		comp = &m_components_list[index_fm1];
		a_doc.AddLine(DD500_FIDMARK(comp->pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	if(index_fm2 >= 0)
	{
		comp = &m_components_list[index_fm2];
		a_doc.AddLine(DD500_FIDMARK(comp->pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	if(index_fm3 >= 0)
	{
		comp = &m_components_list[index_fm3];
		a_doc.AddLine(DD500_FIDMARK(comp->pnp_name, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y)));
	}
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Panel Reference Points"));
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Dispense"));
	a_doc.AddLine(DD500_PARAM("Move height",	"1.500"));
	a_doc.AddLine(wxEmptyString);
	a_doc.AddLine(DD500_SECTION("Components"));
	a_doc.AddLine(";Ref----|-Component name-----------------|----X---|----Y---|-Angle-|Prio|-ID-");
	for(int index = 0; index < comps_count; index++)
	{
		comp = &m_components_list[index];
		if((comp->layer.Upper() != a_side.Upper()) || comp->designator.StartsWith("FM"))
			continue;
		a_doc.AddLine(DD500_COMP(comp->designator, comp->pnp_footprint, DD500_CALC_COORD(comp, x), DD500_CALC_COORD(comp, y), comp->pnp_angle));
	}
	a_doc.AddLine(wxEmptyString);
}

void PnP_convFrame::PrintComponent(t_xml_node_ptrs *a_node, t_component_descr a_comp)
{
	wxString str;
	wxXmlNode *node;
	if(NULL == a_node)
		return;

	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Component");
	node->AddAttribute("Ref", a_comp.designator);
	node->AddAttribute("Name", a_comp.pnp_name);
	str = wxString::Format("%ld,%ld", (long)(a_comp.pnp_location_x*1000), (long)(a_comp.pnp_location_y*1000));
	node->AddAttribute("Position", str);
	str = wxString::Format("%d", (int)(a_comp.pnp_angle*100));
	node->AddAttribute("Angle", str);

	a_node->parent->InsertChildAfter(node, a_node->last_child);
	a_node->last_child = node;
}

void PnP_convFrame::PrintFiducial(t_xml_node_ptrs *a_node, t_component_descr a_comp)
{
	wxString ref_str;
	wxXmlNode *node;
	t_fid_mark_descr *fm_descr;
	if(NULL == a_node)
		return;
	fm_descr = new t_fid_mark_descr;
	fm_descr->designator = a_comp.designator;
	long fm_descr_index = m_fid_marks_list.Index(fm_descr);
	delete fm_descr;
	if(wxNOT_FOUND == fm_descr_index)
		return;
	fm_descr = m_fid_marks_list[fm_descr_index];

	if(FID_MARK_USE_FM1 == fm_descr->usage_type)
	{
		ref_str = "1";
	} else if(FID_MARK_USE_FM2 == fm_descr->usage_type) {
		ref_str = "2";
	} else if(FID_MARK_USE_FM3 == fm_descr->usage_type) {
		ref_str = "3";
	} else {
		return;
	}
	node = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducial");
	node->AddAttribute("Ref", ref_str);
	node->AddAttribute("CadRef", a_comp.designator);
	node->AddAttribute("Position", wxString::Format("%ld,%ld", (long)(a_comp.pnp_location_x*1000), (long)(a_comp.pnp_location_y*1000)));
	node->AddAttribute("Name", a_comp.pnp_name);

	a_node->parent->InsertChildAfter(node, a_node->last_child);
	a_node->last_child = node;
}

wxXmlNode *PnP_convFrame::CreateProductSideDescr(wxString a_side)
{
	wxString ref_str;
	wxXmlNode *side_node, *tmp_node;
	t_component_descr *fm_comp;
	t_fid_mark_descr *fm_descr;

	side_node = new wxXmlNode(wxXML_ELEMENT_NODE, (a_side.Upper() == "BOT")?"BottomSide":"TopSide");
	side_node->AddAttribute("Barcode", "");

	wxXmlNode *prod_orientation_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Orientations");
	side_node->AddChild(prod_orientation_node);
	tmp_node = new wxXmlNode(prod_orientation_node, wxXML_ELEMENT_NODE, "PP050");
	tmp_node->AddAttribute("ProductOrientation", wxString::Format("%d", m_project.angle));

	wxXmlNode *prod_fiducials_node = new wxXmlNode(wxXML_ELEMENT_NODE, "Fiducials");
	side_node->AddChild(prod_fiducials_node);
	prod_fiducials_node->AddAttribute("BadmarkSupport", "no");

	if(m_project.pcbs.GetCount() > 1)
	{
		for(int index = m_fid_marks_list.GetCount()-1; index >= 0; index--)
		{
			fm_descr = m_fid_marks_list[index];
			fm_comp = &m_components_list[fm_descr->component_index];
			if(fm_comp->layer.Upper() != a_side.Upper())
				continue;
			if(FID_MARK_USE_FM1 == fm_descr->usage_as_global)
			{
				ref_str = "1";
			} else if(FID_MARK_USE_FM2 == fm_descr->usage_as_global) {
				ref_str = "2";
			} else if(FID_MARK_USE_FM3 == fm_descr->usage_as_global) {
				ref_str = "3";
			} else {
				continue;
			}
			tmp_node = new wxXmlNode(prod_fiducials_node, wxXML_ELEMENT_NODE, "Fiducial");
			tmp_node->AddAttribute("Ref", ref_str);
			tmp_node->AddAttribute("CadRef", fm_comp->designator);
			tmp_node->AddAttribute("Position", wxString::Format("%ld,%ld", (long)(fm_comp->pnp_location_x*1000), (long)(fm_comp->pnp_location_y*1000)));
			tmp_node->AddAttribute("Name", fm_comp->pnp_name);
		}
	}
	tmp_node = new wxXmlNode(prod_fiducials_node, wxXML_ELEMENT_NODE, "PP050");
	tmp_node->AddAttribute("UseGlobalRefs", "yes");
	tmp_node->AddAttribute("CheckPanelRefs", (m_project.pcbs.GetCount() > 1)?"yes":"no");
	tmp_node->AddAttribute("CheckLocalRefs", "yes");
	tmp_node->AddAttribute("HighAccuracy", "yes");
	return side_node;
}

void PnP_convFrame::ReInitLists()
{
	m_comp_table->ReInit();

	wxGridTableMessage clr1(m_grd_comp_type->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, m_grd_comp_type->GetNumberRows());
	m_grd_comp_type->ProcessTableMessage(clr1);
	wxGridTableMessage add1(m_grd_comp_type->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_APPENDED, m_component_types_list.GetCount());
	m_grd_comp_type->ProcessTableMessage(add1);
	m_grd_comp_type->AutoSize();

	wxGridTableMessage clr2(m_grd_pattern->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, m_grd_pattern->GetNumberRows());
	m_grd_pattern->ProcessTableMessage(clr2);
	wxGridTableMessage add2(m_grd_pattern->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_APPENDED, m_patterns_list.GetCount());
	m_grd_pattern->ProcessTableMessage(add2);
	m_grd_pattern->AutoSize();

	wxGridTableMessage clr3(m_grd_fid_mark->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, m_grd_fid_mark->GetNumberRows());
	m_grd_fid_mark->ProcessTableMessage(clr3);
	wxGridTableMessage add3(m_grd_fid_mark->GetTable(), wxGRIDTABLE_NOTIFY_ROWS_APPENDED, m_fid_marks_list.GetCount());
	m_grd_fid_mark->ProcessTableMessage(add3);
	m_grd_fid_mark->AutoSize();
}

void PnP_convFrame::SaveProjectInfo()
{
	long subpcbs = m_project.pcbs.GetCount();
	long fidmarks = m_fid_marks_list.GetCount();
	wxConfigPathChanger cfg_cd_to(m_cfg_projects, "/"+m_project.filename+"/");
	m_cfg_projects->Write("project_name", m_project.project_name);
	m_cfg_projects->Write("pcb_height", m_project.height);
	m_cfg_projects->Write("pnp_angle", m_project.angle);
	m_cfg_projects->Write("apply_offset", m_project.apply_offset);
//	m_cfg_projects->Write("subpcb_count", subpcbs);
	m_cfg_projects->DeleteGroup("subpcb");
	for (long index = 0; index < subpcbs; index++)
	{
		m_cfg_projects->Write("subpcb/"+m_project.pcbs[index].subpcb_name+"/size_x", m_project.pcbs[index].size_x);
		m_cfg_projects->Write("subpcb/"+m_project.pcbs[index].subpcb_name+"/size_y", m_project.pcbs[index].size_y);
		m_cfg_projects->Write("subpcb/"+m_project.pcbs[index].subpcb_name+"/offset_x", m_project.pcbs[index].offset_x);
		m_cfg_projects->Write("subpcb/"+m_project.pcbs[index].subpcb_name+"/offset_y", m_project.pcbs[index].offset_y);
		m_cfg_projects->Write("subpcb/"+m_project.pcbs[index].subpcb_name+"/enabled", m_project.pcbs[index].enabled);
	}
//Save fidmark info
	m_cfg_projects->DeleteGroup("fid_mark");
	for (long index = 0; index < fidmarks; index++)
	{
		m_cfg_projects->Write("fid_mark/"+m_fid_marks_list[index]->designator+"/usage_type", G_array_on_subpcb[m_fid_marks_list[index]->usage_type]);
		m_cfg_projects->Write("fid_mark/"+m_fid_marks_list[index]->designator+"/usage_as_global", G_array_global[m_fid_marks_list[index]->usage_as_global]);
	}
	m_cfg_projects->Flush();
}

void PnP_convFrame::LoadProjectInfo(wxString a_filename)
{
	if(a_filename.IsEmpty())
		return;
	m_project.filename = a_filename;
	m_project.pcbs.Clear();

	wxConfigPathChanger cfg_cd_to(m_cfg_projects, "/"+m_project.filename+"/");
	m_project.project_name = m_cfg_projects->Read("project_name", m_project.filename.BeforeLast('.'));
	m_project.height = m_cfg_projects->ReadDouble("pcb_height", m_project.height);
	m_project.angle = m_cfg_projects->ReadLong("pnp_angle", m_project.angle);
	m_project.apply_offset = m_cfg_projects->ReadBool("apply_offset", m_project.apply_offset);
	switch(m_project.angle)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		break;
	case 90:
		m_project.angle = 1;
		break;
	case 180:
		m_project.angle = 2;
		break;
	case 270:
		m_project.angle = 3;
		break;
	default:
		m_project.angle = 0;
		break;
	}
//	long subpcbs = m_cfg_projects->ReadLong("subpcb_count", -1);
//	if(subpcbs > 0)
	if(m_cfg_projects->HasGroup("subpcb"))
	{
		wxString subpcb_name;
		long dummy;
		wxConfigPathChanger cfg_cd_to_sub_pcb(m_cfg_projects, "subpcb/");
		for (bool have_group = m_cfg_projects->GetFirstGroup(subpcb_name, dummy);
			have_group;
			have_group = m_cfg_projects->GetNextGroup(subpcb_name, dummy)
		)
		{
			t_subpcb_descr new_subpcb;
			new_subpcb.subpcb_name = subpcb_name;
			new_subpcb.size_x   = m_cfg_projects->ReadDouble(subpcb_name+"/size_x", 100);
			new_subpcb.size_y   = m_cfg_projects->ReadDouble(subpcb_name+"/size_y", 100);
			new_subpcb.offset_x = m_cfg_projects->ReadDouble(subpcb_name+"/offset_x", 0);
			new_subpcb.offset_y = m_cfg_projects->ReadDouble(subpcb_name+"/offset_y", 0);
			new_subpcb.ref_point1_x = new_subpcb.offset_x;
			new_subpcb.ref_point1_y = new_subpcb.offset_y;
			new_subpcb.ref_point2_x = new_subpcb.offset_x + new_subpcb.size_x;
			new_subpcb.ref_point2_y = new_subpcb.offset_y + new_subpcb.size_y;
			new_subpcb.enabled = m_cfg_projects->ReadBool(subpcb_name+"enabled", 1);
			m_project.pcbs.Add(new_subpcb);
		}
	} else {
		t_subpcb_descr new_subpcb;
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
		m_project.pcbs.Add(new_subpcb);
	}

//Load fidmark info
	if(m_cfg_projects->HasGroup("fid_mark"))
	{
		wxString fid_mark_des;
		long dummy;
		wxConfigPathChanger cfg_cd_to_sub_pcb(m_cfg_projects, "fid_mark/");
		for (bool have_group = m_cfg_projects->GetFirstGroup(fid_mark_des, dummy);
			have_group;
			have_group = m_cfg_projects->GetNextGroup(fid_mark_des, dummy)
		)
		{
			wxString tmp_str;
			int str_index;
			t_fid_mark_descr *fid_mark = new t_fid_mark_descr;
			fid_mark->designator = fid_mark_des;
			tmp_str = m_cfg_projects->Read(fid_mark_des+"/usage_type", G_array_on_subpcb[FID_MARK_USE_UNKNOWN]);
			str_index = G_array_on_subpcb.Index(tmp_str, false);
			fid_mark->usage_type = (wxNOT_FOUND == str_index)?FID_MARK_USE_UNKNOWN:str_index;
			tmp_str = m_cfg_projects->Read(fid_mark_des+"/usage_as_global", G_array_global[FID_MARK_USE_UNKNOWN]);
			str_index = G_array_global.Index(tmp_str, false);
			fid_mark->usage_as_global = (wxNOT_FOUND == str_index)?FID_MARK_USE_UNKNOWN:str_index;
			m_fid_marks_list.Add(fid_mark);
		}
	}
//load to GUI
	UpdatePCBFullSize();
	RedrawProjectInfo();
	m_pgProps->SetSplitterLeft();
//	if(need_resave_project)
//		SaveProjectInfo();
}

void PnP_convFrame::RedrawProjectInfo()
{
	wxPGChoices arr_orientation;
	wxBoolProperty *bool_prop;
	arr_orientation.Add("0", 0);
	arr_orientation.Add("90", 1);
	arr_orientation.Add("180", 2);
	arr_orientation.Add("270", 3);
	m_pgProps->Clear();
	m_pgProps->Append( new wxStringProperty("Project", wxPG_LABEL, m_project.project_name) );
	m_pgProps->Append( new wxStringProperty("Filename", wxPG_LABEL, m_project.filename) );		m_pgProps->SetPropertyReadOnly("Filename");
	m_pgProps->Append( new wxFloatProperty("Height", wxPG_LABEL, m_project.height) );
	m_pgProps->Append( new wxEnumProperty("Angle", wxPG_LABEL, arr_orientation, m_project.angle) );
	m_pgProps->Append( new wxFloatProperty("Size X",  wxPG_LABEL, m_project.size_x) );		m_pgProps->SetPropertyReadOnly("Size X");
	m_pgProps->Append( new wxFloatProperty("Size Y", wxPG_LABEL, m_project.size_y) );		m_pgProps->SetPropertyReadOnly("Size Y");
	m_pgProps->Append( new wxFloatProperty("Offset X", wxPG_LABEL, m_project.offset_x) );		m_pgProps->SetPropertyReadOnly("Offset X");
	m_pgProps->Append( new wxFloatProperty("Offset Y", wxPG_LABEL, m_project.offset_y) );		m_pgProps->SetPropertyReadOnly("Offset Y");
	bool_prop = new wxBoolProperty("Apply offset", wxPG_LABEL, m_project.apply_offset);
	bool_prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	m_pgProps->Append( bool_prop );
	long subpcbs = m_project.pcbs.GetCount();
	m_pgProps->Append( new wxIntProperty("SubPCB count", wxPG_LABEL, subpcbs) );
	for (long index = 0; index < subpcbs; index++)
	{
		wxString cat_name = wxString::Format("SubPcb %ld", index);
		wxPGProperty* subpcbProp = m_pgProps->Append( new wxStringProperty(cat_name, wxPG_LABEL, wxEmptyString) );
		m_pgProps->SetPropertyReadOnly(cat_name);
		m_pgProps->AppendIn(subpcbProp, new wxStringProperty("PCB name", wxPG_LABEL, m_project.pcbs[index].subpcb_name) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("size_x", wxPG_LABEL, m_project.pcbs[index].size_x) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("size_y", wxPG_LABEL, m_project.pcbs[index].size_y) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("offset_x", wxPG_LABEL, m_project.pcbs[index].offset_x) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("offset_y", wxPG_LABEL, m_project.pcbs[index].offset_y) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("ref_point1_x", wxPG_LABEL, m_project.pcbs[index].ref_point1_x) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("ref_point1_y", wxPG_LABEL, m_project.pcbs[index].ref_point1_y) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("ref_point2_x", wxPG_LABEL, m_project.pcbs[index].ref_point2_x) );
		m_pgProps->AppendIn(subpcbProp, new wxFloatProperty ("ref_point2_y", wxPG_LABEL, m_project.pcbs[index].ref_point2_y) );
		bool_prop = new wxBoolProperty("Enabled", wxPG_LABEL, m_project.pcbs[index].enabled);
		bool_prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
		m_pgProps->AppendIn(subpcbProp, bool_prop );
	}
}

void PnP_convFrame::OnPropertyGridChanged(wxPropertyGridEvent& a_event)
{
	wxPGProperty* property = a_event.GetProperty();
	// Do nothing if event did not have associated property
	if ( !property )
		return;
	wxString prop_name = property->GetName();
	wxAny value = property->GetValue();
	if ( value.IsNull() )
	{
		wxMessageBox("Value is NULL!", "Warning");
		return;
	}
	// Handle changes in values, as needed
	if ( prop_name == "Project" )
	{
		m_project.project_name = value.As<wxString>();
	} else if ( prop_name == "Height" ) {
		m_project.height = value.As<double>();
	} else if ( prop_name == "Angle" ) {
		m_project.angle = value.As<long>();
	} else if ( prop_name == "Apply offset" ) {
		m_project.apply_offset = value.As<bool>();
	} else if ( prop_name == "SubPCB count" ) {
		int new_subpcb_count = value.As<int>();
		int subpcbs = m_project.pcbs.GetCount();
		if(new_subpcb_count < 1)
		{
			wxMessageBox("SubPcb count must be grater then 0!", "Input error");
			property->SetValue(subpcbs);
			return;
		}
		if(new_subpcb_count < subpcbs)
		{
			if(wxYES != wxMessageBox(wxString::Format("Are you shure that you want to delete %d last subcpbs?", subpcbs - new_subpcb_count), "Warning", wxYES_NO|wxCENTRE))
			{
				property->SetValue(subpcbs);
				return;
			}
			m_project.pcbs.RemoveAt(new_subpcb_count, subpcbs - new_subpcb_count);
		} else {
			for(int index = new_subpcb_count - subpcbs; index > 0; index--)
			{
				t_subpcb_descr new_subpcb;
				new_subpcb.subpcb_name = wxString::Format("SubPCB %d", (int)m_project.pcbs.GetCount());
				new_subpcb.size_x = 100;
				new_subpcb.size_y = 100;
				new_subpcb.offset_x = 0;
				new_subpcb.offset_y = 0;
				new_subpcb.ref_point1_x = new_subpcb.offset_x;
				new_subpcb.ref_point1_y = new_subpcb.offset_y;
				new_subpcb.ref_point2_x = new_subpcb.offset_x + new_subpcb.size_x;
				new_subpcb.ref_point2_y = new_subpcb.offset_y + new_subpcb.size_y;
				new_subpcb.enabled = 1;
				m_project.pcbs.Add(new_subpcb);
			}
		}

	} else if ( prop_name.StartsWith("SubPcb ") ) {
		prop_name = prop_name.AfterFirst('.');
		wxPGProperty* category = property->GetParent();
		if(NULL == category)
			return;
		wxString category_name = category->GetName();
		wxString subpcb_index_str = category_name.AfterLast(' ');
		wxString subpcb_name;
		long subpcb_index;
		subpcb_index_str.ToLong(&subpcb_index);
		t_subpcb_descr &subpcb = m_project.pcbs[subpcb_index];

		if(prop_name == "PCB name")
		{
			subpcb.subpcb_name = value.As<wxString>();
		} else if(prop_name == "enabled") {
			subpcb.enabled = value.As<bool>();
		} else if((prop_name == "size_x")||(prop_name == "size_y")||(prop_name == "offset_x")||(prop_name == "offset_y")) {
			subpcb.size_x   = category->GetPropertyByName("size_x")->GetValue().GetDouble();
			subpcb.size_y   = category->GetPropertyByName("size_y")->GetValue().GetDouble();
			subpcb.offset_x = category->GetPropertyByName("offset_x")->GetValue().GetDouble();
			subpcb.offset_y = category->GetPropertyByName("offset_y")->GetValue().GetDouble();
			subpcb.ref_point1_x = subpcb.offset_x;
			subpcb.ref_point1_y = subpcb.offset_y;
			subpcb.ref_point2_x = subpcb.offset_x + subpcb.size_x;
			subpcb.ref_point2_y = subpcb.offset_y + subpcb.size_y;
			UpdatePCBFullSize();
		} else {
			subpcb.ref_point1_x = category->GetPropertyByName("ref_point1_x")->GetValue().GetDouble();
			subpcb.ref_point1_y = category->GetPropertyByName("ref_point1_y")->GetValue().GetDouble();
			subpcb.ref_point2_x = category->GetPropertyByName("ref_point2_x")->GetValue().GetDouble();
			subpcb.ref_point2_y = category->GetPropertyByName("ref_point2_y")->GetValue().GetDouble();
			if((subpcb.ref_point2_x < subpcb.ref_point1_x)||(subpcb.ref_point2_y < subpcb.ref_point1_y))
			{
				wxMessageBox("Point 1 must be left-bottom and point 2 - right-top!", "Incorrect input");
				subpcb.ref_point1_x = subpcb.offset_x;
				subpcb.ref_point1_y = subpcb.offset_y;
				subpcb.ref_point2_x = subpcb.offset_x + subpcb.size_x;
				subpcb.ref_point2_y = subpcb.offset_y + subpcb.size_y;
				RedrawProjectInfo();
				return;
			}
			subpcb.size_x = subpcb.ref_point2_x - subpcb.ref_point1_x;
			subpcb.size_y = subpcb.ref_point2_y - subpcb.ref_point1_y;
			subpcb.offset_x = subpcb.ref_point1_x;
			subpcb.offset_y = subpcb.ref_point1_y;
			UpdatePCBFullSize();
		}
	}
	SaveProjectInfo();
	RedrawProjectInfo();
	UpdateComponents();
}
void PnP_convFrame::UpdatePCBFullSize()
{
	long subpcbs = m_project.pcbs.GetCount();
	double x1_min = INFINITY, y1_min = INFINITY, x2_max = -INFINITY, y2_max = -INFINITY;

	for (long index = 0; index < subpcbs; index++)
	{
		if(m_project.pcbs[index].ref_point1_x < x1_min) x1_min = m_project.pcbs[index].ref_point1_x;
		if(m_project.pcbs[index].ref_point1_y < y1_min) y1_min = m_project.pcbs[index].ref_point1_y;
		if(m_project.pcbs[index].ref_point2_x > x2_max) x2_max = m_project.pcbs[index].ref_point2_x;
		if(m_project.pcbs[index].ref_point2_y > y2_max) y2_max = m_project.pcbs[index].ref_point2_y;
	}
	m_project.size_x = x2_max - x1_min;
	m_project.size_y = y2_max - y1_min;
	m_project.offset_x = x1_min;
	m_project.offset_y = y1_min;
}
bool PnP_convFrame::IsInRoom(double a_ref1_x, double a_ref1_y, double a_ref2_x, double a_ref2_y, double a_ptr_x, double a_ptr_y)
{
	double tmp;
	bool res;
	if(a_ref1_x > a_ref2_x)
	{
		tmp = a_ref2_x;
		a_ref2_x = a_ref1_x;
		a_ref1_x = tmp;
	}
	if(a_ref1_y > a_ref2_y)
	{
		tmp = a_ref2_y;
		a_ref2_y = a_ref1_y;
		a_ref1_y = tmp;
	}
	res   = (a_ptr_x >= a_ref1_x) && (a_ptr_x <= a_ref2_x) &&
		(a_ptr_y >= a_ref1_y) && (a_ptr_y <= a_ref2_y);
	return res;
}
