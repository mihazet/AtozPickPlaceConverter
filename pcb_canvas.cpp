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

	// высчитываем минимальный масштаб, что бы вся плата влезла на зкран
	double scale = wxMin((double) w / (double) w_pcb, (double) h / (double) h_pcb);
	// устанавливаем по обоим осям одинаковый масштаб,
	// чтобы не нарушались пропорции
	dc.SetUserScale(scale, scale);

	// переводим теперь физические размеры экрана в логические
	// и дальше работаем в логических
	int w_log = dc.DeviceToLogicalXRel(w);
	int h_log = dc.DeviceToLogicalYRel(h);

	// рисуем общую плату по центру
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(wxBrush(wxColour("FOREST GREEN")));
	dc.DrawRectangle((w_log - w_pcb) / 2, (h_log - h_pcb) / 2, w_pcb, h_pcb);

	// рисуем входящие платы
	if (!m_project->IsSingleBoard())
	{
	}

}

void PcbCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
}
