#ifndef PCB_CANVAS_H
#define PCB_CANVAS_H

#include <wx/wx.h>

class Project;

class PcbCanvas : public wxWindow
{
public:
	PcbCanvas(wxWindow *parent, wxWindowID id, Project *project);

private:
	Project *m_project;
};

#endif
