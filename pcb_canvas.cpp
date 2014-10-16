#include "pcb_canvas.h"
#include "project.h"

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

	wxSize clientSize = GetClientSize();

	// default init
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxBLACK_PEN);
	dc.Clear();

	// Инициализация направления осей
	// X: слева направо
	// Y: снизу вверх
	// (0,0): в левом нижнем углу
	dc.SetAxisOrientation(true, true);
	dc.SetDeviceOrigin(0, clientSize.GetHeight());

	// физические размеры экрана
	int w = clientSize.GetWidth();
	int h = clientSize.GetHeight();

	// размеры платы в тысячных долях миллиметра
	int w_pcb = m_project->SizeX() * 1000;
	int h_pcb = m_project->SizeY() * 1000;
	// смещение платы
	int offset_x_pcb = m_project->OffsetX() * 1000;
	int offset_y_pcb = m_project->OffsetY() * 1000;

	// высчитываем минимальный масштаб, что бы вся плата вместе
	// со смещением от нуля влезла на зкран
	double scale = wxMin((double) w / (double) (w_pcb + offset_x_pcb),
						 (double) h / (double) (h_pcb + offset_y_pcb));

	// пишем ноль
	wxString nullStr = "0";
	wxSize textSize = dc.GetTextExtent(nullStr);
	dc.DrawText(nullStr, 0, textSize.GetHeight());

	// устанавливаем по обоим осям одинаковый масштаб,
	// чтобы не нарушались пропорции
	dc.SetUserScale(scale, scale);

	// рисуем общую плату
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(wxBrush(wxColour("FOREST GREEN")));
	dc.DrawRectangle(offset_x_pcb, offset_y_pcb, w_pcb, h_pcb);

	// рисуем входящие платы
	if (!m_project->IsSingleBoard())
	{
		dc.SetBrush(wxBrush(wxColour("GREEN")));
		for (size_t i = 0; i < m_project->Pcbs().size(); i++)
		{
			int x = m_project->Pcbs()[i].offset_x * 1000;
			int y = m_project->Pcbs()[i].offset_y * 1000;
			int w = m_project->Pcbs()[i].size_x * 1000;
			int h = m_project->Pcbs()[i].size_y * 1000;
			dc.DrawRectangle(x, y, w, h);
		}
	}

}

void PcbCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
}
