#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/propgrid/propgrid.h>
#include <wx/notebook.h>
#include <wx/treectrl.h>
#include <wx/laywin.h>

class Project;

class MainFrame: public wxFrame
{
public:
    MainFrame();
	~MainFrame();

	void CreateMenu();

private:
	void OnFileOpen(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileProjectOpen(wxCommandEvent& event);
	void OnFileProjectSave(wxCommandEvent& event);
	void OnFileQuit(wxCommandEvent& event);
	void OnHelpAbout(wxCommandEvent& event);
	void OnSashDrag(wxSashEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPropertyGridChanged(wxPropertyGridEvent& event);
	void OnTreeActivated(wxTreeEvent& event);
	void OnCompGridColSort(wxGridEvent& event);
	void OnTypeGridColSort(wxGridEvent& event);
	void OnPatternGridColSort(wxGridEvent& event);
	void OnFidmarkGridColSort(wxGridEvent& event);


	void OnProjectLoaded(wxCommandEvent& event);
	void OnProjectCadFileLoaded(wxCommandEvent& event);
	void OnProjectUpdated(wxCommandEvent& event);
	void OnProjectUpdateTitle(wxCommandEvent& event);

	void BuildFilesTree();
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

	wxTreeCtrl *m_filesTree;
	wxPropertyGrid *m_projectPG;
	wxNotebook *m_book;
	wxGrid *m_compGrid;
	wxGrid *m_typeGrid;
	wxGrid *m_patternGrid;
	wxGrid *m_fidMarkGrid;
	wxTextCtrl *m_logText;

	wxSashLayoutWindow *m_leftLayoutWin;
	wxSashLayoutWindow *m_rightLayoutWin;
	wxSashLayoutWindow *m_bottomLayoutWin;


	DECLARE_EVENT_TABLE()
};

