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
#include "comp_table.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include "comp_type_table.h"
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/textctrl.h>
#include "pattern_table.h"
#include <wx/propgrid/propgrid.h>
//*)

#include "common.h"
class wxXmlNode;
class wxFileConfig;

class PnP_convFrame: public wxFrame
{
    public:
	PnP_convFrame(wxWindow* parent,wxWindowID id = -1);
	virtual ~PnP_convFrame();

    private:
    	wxString		m_filename;
        wxFileConfig		*m_config;
	tComponentDescr		m_components_list;
	tComponentTypeDescr	m_component_types_list;
	tPatternDescr		m_patterns_list;

	wxString RemoveQuotes(const wxString a_str);
	void PrintComponent(t_xml_node_ptrs *a_node, t_component_descr a_comp);
	void PrintFiducial(t_xml_node_ptrs *a_node, t_component_descr a_comp);
	void ReInitLists() {m_comp_table->ReInit();m_comp_type_table->ReInit();m_pattern_table->ReInit();};

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

        //(*Identifiers(PnP_convFrame)
        static const long ID_TEXTCTRL1;
        static const long ID_PROP;
        static const long ID_COMP_TABLE;
        static const long ID_PANEL4;
        static const long ID_COMP_TYPE_TABLE;
        static const long ID_PANEL1;
        static const long ID_PATTERNS_TABLE;
        static const long ID_PANEL2;
        static const long ID_AUINOTEBOOK1;
        static const long ID_OPEN;
        static const long ID_SAVE_PROD;
        static const long ID_QUIT;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(PnP_convFrame)
        wxPanel* Panel1;
        wxStatusBar* StatusBar1;
        wxPropertyGrid* m_pgProps;
        wxAuiManager* auiManager;
        wxPanel* Panel2;
        wxTextCtrl* m_txtLog;
        wxPanel* Panel4;
        wxAuiNotebook* auiMainNotebook;
        cCompTypeTable* m_comp_type_table;
        cPatternTable* m_pattern_table;
        cCompTable* m_comp_table;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // IKZ_LOG_VIEWERMAIN_H
