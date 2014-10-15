#include "pcb_canvas.h"

PcbCanvas::PcbCanvas(wxWindow *parent, wxWindowID id, Project *project)
	: wxWindow(parent, id)
	, m_project(project)
{
}
