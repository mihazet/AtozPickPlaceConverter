/***************************************************************
 * Name:      main_frame.h
 * Purpose:   Defines Application Frame
 * Author:    Alatar, MZet
 * Created:   2012-12-16
 * Copyright: Alatar, MZet
 * License:
 **************************************************************/

#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/propgrid/propgrid.h>
#include <wx/notebook.h>

class Project;

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

	void CreateMenu();

private:
	void OnFileOpen(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileQuit(wxCommandEvent& event);
	void OnHelpAbout(wxCommandEvent& event);
	void OnPropertyGridChanged(wxPropertyGridEvent& event);
	void OnCompGridColSort(wxGridEvent& event);
	void OnTypeGridColSort(wxGridEvent& event);
	void OnPatternGridColSort(wxGridEvent& event);
	void OnFidmarkGridColSort(wxGridEvent& event);


	void OnProjectLoaded(wxCommandEvent& event);
	void OnProjectUpdated(wxCommandEvent& event);

	void BuildCompGrid();
	void BuildTypeGrid();
	void BuildPatternGrid();
	void BuildFidMarkGrid();
	void BuildProjectPG();

	void UpdateCompGrid();
	void UpdateTypeGrid();
	void UpdatePatternGrid();
	void UpdateFidMarkGrid();


	Project *m_project;

	wxPropertyGrid *m_projectPG;
	wxNotebook *m_book;
	wxGrid *m_compGrid;
	wxGrid *m_typeGrid;
	wxGrid *m_patternGrid;
	wxGrid *m_fidMarkGrid;
	wxTextCtrl *m_logText;

	DECLARE_EVENT_TABLE()
};

#endif // MAIN_FRAME_H
