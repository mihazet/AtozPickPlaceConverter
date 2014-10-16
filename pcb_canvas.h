#ifndef PCB_CANVAS_H
#define PCB_CANVAS_H

#include <wx/wx.h>

class Project;

class PcbCanvas : public wxWindow
{
public:
	PcbCanvas(wxWindow *parent, wxWindowID id, Project *project);

private:
	void DrawComponent(wxDC& dc, int x, int y);

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);


	Project *m_project;

	DECLARE_EVENT_TABLE()
};

#endif
