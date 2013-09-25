/***************************************************************
 * Name:      PnP_convMain.h
 * Purpose:   Defines Application Frame
 * Author:    Alatar ()
 * Created:   2012-12-16
 * Copyright: Alatar ()
 * License:
 **************************************************************/

#ifndef PNP_CONV_MAIN_H
#define PNP_CONV_MAIN_H

//(*Headers(PnP_convFrame)
#include <wx/grid.h>
#include "comp_table.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/textctrl.h>
#include <wx/propgrid/propgrid.h>
//*)

#include "common.h"
class wxXmlNode;
class wxFileConfig;
class cCompTypeTable;
class cPatternTable;
class cFidMarkTable;

class PnP_convFrame: public wxFrame
{
    public:
	PnP_convFrame(wxWindow* parent,wxWindowID id = -1);
	virtual ~PnP_convFrame();

    private:
	wxFileConfig		*m_cfg_settings;
	wxFileConfig		*m_cfg_components_pcad;
	wxFileConfig		*m_cfg_components_altium;
	wxFileConfig		*m_cfg_patterns_pcad;
	wxFileConfig		*m_cfg_patterns_altium;
	wxFileConfig		*m_cfg_projects;

	t_board_descr		m_project;
	tComponentDescr		m_components_list;
	tComponentTypeDescr	m_component_types_list;
	tPatternDescr		m_patterns_list;
	tFidMarkDescr		m_fid_marks_list;
	cCompTypeTable		*m_component_types_table;
	cPatternTable		*m_pattern_table;
	cFidMarkTable		*m_fid_marks_table;

	wxString RemoveQuotes(const wxString a_str);
	void PrintComponent(t_xml_node_ptrs *a_node, t_component_descr a_comp);
	void PrintFiducial(t_xml_node_ptrs *a_node, t_component_descr a_comp);
	wxXmlNode *CreateProductSideDescr(wxString a_side);
	bool ParseNominals(t_component_type_descr *a_component_type, wxString a_designator, wxString a_value);
	bool NormalizeNominal(t_component_type_descr *a_component_type);
	void UpdateComponents();
	bool UpdateComponent(t_component_descr *a_component, size_t a_comp_index);
	void ReInitLists();
	void RedrawProjectInfo();
	void LoadProjectInfo(wxString a_filename);
	void SaveProjectInfo();
	void UpdatePCBFullSize();
	bool IsInRoom(double a_ref1_x, double a_ref1_y, double a_ref2_x, double a_ref2_y, double a_ptr_x, double a_ptr_y);


        //(*Handlers(PnP_convFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        void OnDevSNSelect(wxCommandEvent& event);
        void On_ShowPointsSelected(wxCommandEvent& event);
        void OnAuiManager1PaneButton(wxAuiManagerEvent& event);
        void OnAuiNotebook1PageClose(wxAuiNotebookEvent& event);
        void OnDevSNDClick(wxCommandEvent& event);
        void On_mnuOpenSelected(wxCommandEvent& event);
        void On_mnuSaveProdSelected(wxCommandEvent& event);
        //*)
	void OnPropertyGridChanged(wxPropertyGridEvent& a_event);

        //(*Identifiers(PnP_convFrame)
        static const long ID_TEXTCTRL1;
        static const long ID_PROP;
        static const long ID_COMP_TABLE;
        static const long ID_PANEL_COMP;
        static const long ID_GRID_COMP_TYPE;
        static const long ID_PANEL_COMP_TYPE;
        static const long ID_GRID_PATTERN;
        static const long ID_PANEL_PAT;
        static const long ID_GRID_FID_MARKS;
        static const long ID_PANEL_FID;
        static const long ID_AUINOTEBOOK1;
        static const long ID_OPEN;
        static const long ID_SAVE_PROD;
        static const long ID_QUIT;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(PnP_convFrame)
        wxGrid* m_grd_pattern;
        wxStatusBar* StatusBar1;
        wxGrid* m_grd_fid_mark;
        wxPropertyGrid* m_pgProps;
        wxAuiManager* auiManager;
        wxTextCtrl* m_txtLog;
        wxGrid* m_grd_comp_type;
        wxAuiNotebook* auiMainNotebook;
        cCompTable* m_comp_table;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // IKZ_LOG_VIEWERMAIN_H
