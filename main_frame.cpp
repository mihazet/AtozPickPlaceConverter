/***************************************************************
 * Name:      main_frame.cpp
 * Purpose:   Code for Application Frame
 * Author:    Alatar, MZet
 * Created:   2012-12-16
 * Copyright: Alatar, MZet
 * License:
 **************************************************************/

#include "main_frame.h"
#include "project.h"
#include "pcb_canvas.h"

#include "comp_table.h"
#include "comp_type_table.h"
#include "pattern_table.h"
#include "fid_mark_table.h"


#define APP_TITLE	("Pick & Place Converter")

enum
{
	ID_FILE_OPEN = wxID_HIGHEST,
	ID_FILE_SAVE,
	ID_FILE_QUIT,
	ID_HELP_ABOUT,
	ID_PROP,
	ID_COMP_GRID,
	ID_TYPE_GRID,
	ID_PATTERN_GRID,
	ID_FIDMARK_GRID,


	ID_WINDOW_RIGHT,
	ID_WINDOW_BOTTOM,
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_FILE_OPEN, MainFrame::OnFileOpen)
	EVT_MENU(ID_FILE_SAVE, MainFrame::OnFileSave)
	EVT_MENU(ID_FILE_QUIT, MainFrame::OnFileQuit)
	EVT_MENU(ID_HELP_ABOUT, MainFrame::OnHelpAbout)
	EVT_SASH_DRAGGED_RANGE(ID_WINDOW_RIGHT, ID_WINDOW_BOTTOM, MainFrame::OnSashDrag)
	EVT_SIZE(MainFrame::OnSize)
	EVT_PG_CHANGED(ID_PROP, MainFrame::OnPropertyGridChanged)
	EVT_GRID_CMD_COL_SORT(ID_COMP_GRID, MainFrame::OnCompGridColSort)
	EVT_GRID_CMD_COL_SORT(ID_TYPE_GRID, MainFrame::OnTypeGridColSort)
	EVT_GRID_CMD_COL_SORT(ID_PATTERN_GRID, MainFrame::OnPatternGridColSort)
	EVT_GRID_CMD_COL_SORT(ID_FIDMARK_GRID, MainFrame::OnFidmarkGridColSort)

	EVT_PROJECT_LOADED(MainFrame::OnProjectLoaded)
	EVT_PROJECT_UPDATED(MainFrame::OnProjectUpdated)

END_EVENT_TABLE()

MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, APP_TITLE)
{
	CreateMenu();

	m_project = new Project();
	PushEventHandler(m_project);

	// window right
	m_rightLayoutWin = new wxSashLayoutWindow(this, ID_WINDOW_RIGHT);
	m_rightLayoutWin->SetDefaultSize(wxSize(200, 1000));
	m_rightLayoutWin->SetOrientation(wxLAYOUT_VERTICAL);
	m_rightLayoutWin->SetAlignment(wxLAYOUT_RIGHT);
	m_rightLayoutWin->SetSashVisible(wxSASH_LEFT, true);

	// window bottom
	m_bottomLayoutWin = new wxSashLayoutWindow(this, ID_WINDOW_BOTTOM);
	m_bottomLayoutWin->SetDefaultSize(wxSize(1000, 100));
	m_bottomLayoutWin->SetOrientation(wxLAYOUT_HORIZONTAL);
	m_bottomLayoutWin->SetAlignment(wxLAYOUT_BOTTOM);
	m_bottomLayoutWin->SetSashVisible(wxSASH_TOP, true);

	m_projectPG = new wxPropertyGrid(m_rightLayoutWin, ID_PROP);
	m_book = new wxNotebook(this, wxID_ANY);
	m_compGrid = new wxGrid(m_book, ID_COMP_GRID);
	m_typeGrid = new wxGrid(m_book, ID_TYPE_GRID);
	m_patternGrid = new wxGrid(m_book, ID_PATTERN_GRID);
	m_fidMarkGrid = new wxGrid(m_book, ID_FIDMARK_GRID);
	m_canvas = new PcbCanvas(m_book, wxID_ANY, m_project);
	m_book->AddPage(m_compGrid, "Components", true);
	m_book->AddPage(m_typeGrid, "Component Types");
	m_book->AddPage(m_patternGrid, "Patterns");
	m_book->AddPage(m_fidMarkGrid, "Fiducials");
	m_book->AddPage(m_canvas, "PCB View");
	m_logText = new wxTextCtrl(m_bottomLayoutWin, wxID_ANY, "",
							   wxDefaultPosition, wxDefaultSize,
							   wxTE_READONLY | wxTE_MULTILINE);

	wxLog::SetActiveTarget(new wxLogTextCtrl(m_logText));

	SetSize(wxSize(1000, 800));
	Center();
}

MainFrame::~MainFrame()
{
	if (m_project)
	{
		PopEventHandler();
		delete m_project;
	}
}

void MainFrame::CreateMenu()
{
	wxMenu *fileMenu = new wxMenu();
	fileMenu->Append(ID_FILE_OPEN, "&Open PnP file...\tCtrl+O");
	fileMenu->Append(ID_FILE_SAVE, "&Save prod file...\tCtrl+S");
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_FILE_QUIT, "&Quit\tCtrl+Q");

	wxMenu *helpMenu = new wxMenu();
	helpMenu->Append(ID_HELP_ABOUT, "About...\tF1");

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);
}

void MainFrame::OnFileOpen(wxCommandEvent &event)
{
	wxFileDialog openDialog(this, "Select a file", "", "",
							"All supported files (*.txt;*.csv)|*.txt;*.csv"
							"|PCAD PnP files (*.txt)|*.txt"
							"|Altium Designer PnP files (*.csv)|*.csv"
							"|All files (*.*)|*.*",
							wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openDialog.ShowModal() == wxID_OK)
	{
		m_project->Load(openDialog.GetPath());
	}
}

void MainFrame::OnFileSave(wxCommandEvent &event)
{
	wxFileName filename(m_project->Filename());
	filename.SetName(m_project->Name());
	filename.SetExt("prod");
	wxFileDialog saveDialog(this, "Enter target filename",
							wxEmptyString, filename.GetFullPath(),
							"PP-050 files (*.prod)|*.prod|DD-500 files (*.pcb)|*.pcb",
							wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveDialog.ShowModal() == wxID_OK)
	{
		m_project->SaveProd(saveDialog.GetPath());
	}
}

void MainFrame::OnFileQuit(wxCommandEvent &event)
{
	Close();
}

void MainFrame::OnHelpAbout(wxCommandEvent& event)
{
	wxMessageBox("PickPlaceConverter Version 1.0\nBuild at "__DATE__" "__TIME__, "About");
}

void MainFrame::OnSashDrag(wxSashEvent& event)
{
	if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
		return;

	switch (event.GetId())
	{
		case ID_WINDOW_RIGHT:
			m_rightLayoutWin->SetDefaultSize(wxSize(event.GetDragRect().width, 1000));
			break;

		case ID_WINDOW_BOTTOM:
			m_bottomLayoutWin->SetDefaultSize(wxSize(1000, event.GetDragRect().height));
			break;
	}

	wxLayoutAlgorithm layout;
	layout.LayoutFrame(this, m_book);
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	wxLayoutAlgorithm layout;
	layout.LayoutFrame(this, m_book);
}

void MainFrame::OnPropertyGridChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
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
		m_project->Name() = value.As<wxString>();
	}
	else if ( prop_name == "Height" )
	{
		m_project->Height() = value.As<double>();
	}
	else if ( prop_name == "Angle" )
	{
		m_project->Angle() = value.As<long>();
	}
	else if ( prop_name == "Apply offset" )
	{
		m_project->ApplyOffset() = value.As<bool>();
	}
	else if ( prop_name == "SubPCB count" )
	{
		int new_subpcb_count = value.As<int>();
		int subpcbs = m_project->Pcbs().size();
		if (new_subpcb_count < 1)
		{
			wxMessageBox("SubPcb count must be grater then 0!", "Input error");
			property->SetValue(subpcbs);
			return;
		}

		if (new_subpcb_count < subpcbs)
		{
			if (wxYES != wxMessageBox(wxString::Format("Are you shure that you want to delete %d last subcpbs?", subpcbs - new_subpcb_count), "Warning", wxYES_NO | wxCENTRE))
			{
				property->SetValue(subpcbs);
				return;
			}
			//m_project->Pcbs().RemoveAt(new_subpcb_count, subpcbs - new_subpcb_count);
			SubpcbVector::iterator start_it = m_project->Pcbs().begin();
			std::advance(start_it, new_subpcb_count);
			m_project->Pcbs().erase(start_it, m_project->Pcbs().end());
		}
		else
		{
			for (int index = new_subpcb_count - subpcbs; index > 0; index--)
			{
				Subpcb new_subpcb;
				new_subpcb.subpcb_name = wxString::Format("SubPCB %d", (int)m_project->Pcbs().size());
				new_subpcb.size_x = 100;
				new_subpcb.size_y = 100;
				new_subpcb.offset_x = 0;
				new_subpcb.offset_y = 0;
				new_subpcb.ref_point1_x = new_subpcb.offset_x;
				new_subpcb.ref_point1_y = new_subpcb.offset_y;
				new_subpcb.ref_point2_x = new_subpcb.offset_x + new_subpcb.size_x;
				new_subpcb.ref_point2_y = new_subpcb.offset_y + new_subpcb.size_y;
				new_subpcb.enabled = 1;
				m_project->Pcbs().push_back(new_subpcb);
			}
		}

	}
	else if ( prop_name.StartsWith("SubPcb ") )
	{
		prop_name = prop_name.AfterFirst('.');
		wxPGProperty* category = property->GetParent();
		if (NULL == category)
			return;
		wxString category_name = category->GetName();
		wxString subpcb_index_str = category_name.AfterLast(' ');
		wxString subpcb_name;
		long subpcb_index;
		subpcb_index_str.ToLong(&subpcb_index);
		Subpcb& subpcb = m_project->Pcbs()[subpcb_index];

		if (prop_name == "PCB name")
		{
			subpcb.subpcb_name = value.As<wxString>();
		}
		else if (prop_name == "enabled")
		{
			subpcb.enabled = value.As<bool>();
		}
		else if ((prop_name == "size_x") || (prop_name == "size_y") || (prop_name == "offset_x") || (prop_name == "offset_y"))
		{
			subpcb.size_x   = category->GetPropertyByName("size_x")->GetValue().GetDouble();
			subpcb.size_y   = category->GetPropertyByName("size_y")->GetValue().GetDouble();
			subpcb.offset_x = category->GetPropertyByName("offset_x")->GetValue().GetDouble();
			subpcb.offset_y = category->GetPropertyByName("offset_y")->GetValue().GetDouble();
			subpcb.ref_point1_x = subpcb.offset_x;
			subpcb.ref_point1_y = subpcb.offset_y;
			subpcb.ref_point2_x = subpcb.offset_x + subpcb.size_x;
			subpcb.ref_point2_y = subpcb.offset_y + subpcb.size_y;
			m_project->UpdatePCBFullSize();
		}
		else
		{
			subpcb.ref_point1_x = category->GetPropertyByName("ref_point1_x")->GetValue().GetDouble();
			subpcb.ref_point1_y = category->GetPropertyByName("ref_point1_y")->GetValue().GetDouble();
			subpcb.ref_point2_x = category->GetPropertyByName("ref_point2_x")->GetValue().GetDouble();
			subpcb.ref_point2_y = category->GetPropertyByName("ref_point2_y")->GetValue().GetDouble();
			if ((subpcb.ref_point2_x < subpcb.ref_point1_x) || (subpcb.ref_point2_y < subpcb.ref_point1_y))
			{
				wxMessageBox("Point 1 must be left-bottom and point 2 - right-top!", "Incorrect input");
				subpcb.ref_point1_x = subpcb.offset_x;
				subpcb.ref_point1_y = subpcb.offset_y;
				subpcb.ref_point2_x = subpcb.offset_x + subpcb.size_x;
				subpcb.ref_point2_y = subpcb.offset_y + subpcb.size_y;
				m_project->Notify(wxEVT_PROJECT_UPDATED);
				return;
			}
			subpcb.size_x = subpcb.ref_point2_x - subpcb.ref_point1_x;
			subpcb.size_y = subpcb.ref_point2_y - subpcb.ref_point1_y;
			subpcb.offset_x = subpcb.ref_point1_x;
			subpcb.offset_y = subpcb.ref_point1_y;
			m_project->UpdatePCBFullSize();
		}
	}
	m_project->UpdateComponents();
	m_project->Notify(wxEVT_PROJECT_UPDATED);
}

void MainFrame::OnCompGridColSort(wxGridEvent& event)
{
	int col = event.GetCol();

	ComponentTable *table = wxDynamicCast(m_compGrid->GetTable(), ComponentTable);
	if (table && !m_compGrid->IsSortingBy(col))
	{
		table->Sort(col);
	}
}

void MainFrame::OnTypeGridColSort(wxGridEvent& event)
{
	int col = event.GetCol();

	ComponentTypeTable *table = wxDynamicCast(m_typeGrid->GetTable(), ComponentTypeTable);
	if (table && !m_typeGrid->IsSortingBy(col))
	{
		table->Sort(col);
	}
}

void MainFrame::OnPatternGridColSort(wxGridEvent& event)
{
	int col = event.GetCol();

	PatternTable *table = wxDynamicCast(m_patternGrid->GetTable(), PatternTable);
	if (table && !m_patternGrid->IsSortingBy(col))
	{
		table->Sort(col);
	}
}

void MainFrame::OnFidmarkGridColSort(wxGridEvent& event)
{
	int col = event.GetCol();

	FidMarkTable *table = wxDynamicCast(m_fidMarkGrid->GetTable(), FidMarkTable);
	if (table && !m_fidMarkGrid->IsSortingBy(col))
	{
		table->Sort(col);
	}
}



void MainFrame::OnProjectLoaded(wxCommandEvent& event)
{
	BuildCompGrid();
	BuildTypeGrid();
	BuildPatternGrid();
	BuildFidMarkGrid();

	BuildProjectPG();

	m_canvas->Refresh();

	// Update frame title
	SetTitle(wxFileNameFromPath(m_project->Filename()) + " - " + APP_TITLE);
	wxLogMessage("Done.");
}

void MainFrame::OnProjectUpdated(wxCommandEvent& event)
{
	BuildProjectPG();
	UpdateCompGrid();
	UpdateTypeGrid();
	UpdatePatternGrid();
	UpdateFidMarkGrid();
	wxLogMessage("Done.");
}

void MainFrame::BuildCompGrid()
{
	wxLogMessage("Build components grid.");
	m_compGrid->SetTable( new ComponentTable(m_project), true );
	m_compGrid->AutoSizeColumns();
	m_compGrid->ForceRefresh();
}

void MainFrame::BuildTypeGrid()
{
	wxLogMessage("Build types grid.");
	m_typeGrid->SetTable( new ComponentTypeTable(m_project), true );
	m_typeGrid->AutoSizeColumns();
	m_typeGrid->ForceRefresh();
}

void MainFrame::BuildPatternGrid()
{
	wxLogMessage("Build patterns grid.");
	m_patternGrid->SetTable( new PatternTable(m_project), true );
	m_patternGrid->AutoSizeColumns();
	m_patternGrid->ForceRefresh();
}

void MainFrame::BuildFidMarkGrid()
{
	wxLogMessage("Build fidmarks grid.");
	m_fidMarkGrid->SetTable( new FidMarkTable(m_project), true );
	m_fidMarkGrid->AutoSizeColumns();
	m_fidMarkGrid->ForceRefresh();
}

void MainFrame::BuildProjectPG()
{
	wxLogMessage("Build property grid.");

	m_projectPG->Freeze();

	wxPGChoices arr_orientation;
	wxBoolProperty *bool_prop;
	arr_orientation.Add("0", 0);
	arr_orientation.Add("90", 1);
	arr_orientation.Add("180", 2);
	arr_orientation.Add("270", 3);
	m_projectPG->Clear();
	m_projectPG->Append( new wxStringProperty("Project", wxPG_LABEL, m_project->Name()) );
	m_projectPG->Append( new wxStringProperty("Filename", wxPG_LABEL, wxFileNameFromPath(m_project->Filename())) );
	m_projectPG->SetPropertyReadOnly("Filename");
	m_projectPG->Append( new wxFloatProperty("Height", wxPG_LABEL, m_project->Height()) );
	m_projectPG->Append( new wxEnumProperty("Angle", wxPG_LABEL, arr_orientation, m_project->Angle()) );
	m_projectPG->Append( new wxFloatProperty("Size X",  wxPG_LABEL, m_project->SizeX()) );
	m_projectPG->SetPropertyReadOnly("Size X");
	m_projectPG->Append( new wxFloatProperty("Size Y", wxPG_LABEL, m_project->SizeY()) );
	m_projectPG->SetPropertyReadOnly("Size Y");
	m_projectPG->Append( new wxFloatProperty("Offset X", wxPG_LABEL, m_project->OffsetX()) );
	m_projectPG->SetPropertyReadOnly("Offset X");
	m_projectPG->Append( new wxFloatProperty("Offset Y", wxPG_LABEL, m_project->OffsetY()) );
	m_projectPG->SetPropertyReadOnly("Offset Y");
	bool_prop = new wxBoolProperty("Apply offset", wxPG_LABEL, m_project->ApplyOffset());
	bool_prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	m_projectPG->Append( bool_prop );
	long subpcbs = m_project->Pcbs().size();
	m_projectPG->Append( new wxIntProperty("SubPCB count", wxPG_LABEL, subpcbs) );

	SubpcbVector::iterator it;
	for (it = m_project->Pcbs().begin(); it != m_project->Pcbs().end(); it++)
	{
		wxString cat_name = wxString::Format("SubPcb %ld", std::distance(m_project->Pcbs().begin(), it));
		wxPGProperty* subpcbProp = m_projectPG->Append( new wxStringProperty(cat_name, wxPG_LABEL, wxEmptyString) );
		m_projectPG->SetPropertyReadOnly(cat_name);
		m_projectPG->AppendIn(subpcbProp, new wxStringProperty("PCB name", wxPG_LABEL, it->subpcb_name) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("size_x", wxPG_LABEL, it->size_x) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("size_y", wxPG_LABEL, it->size_y) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("offset_x", wxPG_LABEL, it->offset_x) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("offset_y", wxPG_LABEL, it->offset_y) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("ref_point1_x", wxPG_LABEL, it->ref_point1_x) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("ref_point1_y", wxPG_LABEL, it->ref_point1_y) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("ref_point2_x", wxPG_LABEL, it->ref_point2_x) );
		m_projectPG->AppendIn(subpcbProp, new wxFloatProperty ("ref_point2_y", wxPG_LABEL, it->ref_point2_y) );
		bool_prop = new wxBoolProperty("Enabled", wxPG_LABEL, it->enabled);
		bool_prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
		m_projectPG->AppendIn(subpcbProp, bool_prop );
	}

	m_projectPG->SetSplitterLeft();
	m_projectPG->Thaw();
}

void MainFrame::UpdateCompGrid()
{
	wxLogMessage("Update components grid.");
	m_compGrid->ForceRefresh();
}

void MainFrame::UpdateTypeGrid()
{
	wxLogMessage("Update types grid.");
	m_typeGrid->ForceRefresh();
}

void MainFrame::UpdatePatternGrid()
{
	wxLogMessage("Update patterns grid.");
	m_patternGrid->ForceRefresh();
}

void MainFrame::UpdateFidMarkGrid()
{
	wxLogMessage("Update fimakrs grid.");
	m_fidMarkGrid->ForceRefresh();
}


