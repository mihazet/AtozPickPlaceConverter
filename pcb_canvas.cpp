#include "pcb_canvas.h"

BEGIN_EVENT_TABLE(PcbCanvas, wxWindow)
	EVT_PAINT(PcbCanvas::OnPaint)
	EVT_SIZE(PcbCanvas::OnSize)
END_EVENT_TABLE()

PcbCanvas::PcbCanvas(wxWindow *parent, wxWindowID id, Project *project)
	: wxWindow(parent, id)
	, m_project(project)
{
}

void PcbCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxRect clientRect = GetClientRect();
	clientRect.Deflate(10);
	// default init
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxBLACK_PEN);
	dc.Clear();

	dc.SetPen(*wxBLUE_PEN);
	dc.DrawRectangle(clientRect);
}

void PcbCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
}
