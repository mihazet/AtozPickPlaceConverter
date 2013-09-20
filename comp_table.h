#ifndef COMP_TABLE_H
#define COMP_TABLE_H

#include "common.h"
#include <wx/wx.h>
#include <wx/listctrl.h>

class cCompTable : public wxListCtrl
{
public:
	cCompTable();
	cCompTable(tComponentDescr *a_data, wxWindow *parent, wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxLC_REPORT|wxLC_VIRTUAL|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES,
		const wxValidator &validator = wxDefaultValidator,
		const wxString &name = wxListCtrlNameStr);
	virtual ~cCompTable();
	void ReInit();
protected:
private:
	tComponentDescr		*m_comp_data;
//	wxTimer			*m_Timer;

	// Переопределяем виртуальные функции

	// ----------------------------------------------------------------------------
	// Текст в ячейках
	// ----------------------------------------------------------------------------
	virtual wxString OnGetItemText(long item, long column) const;

	// ----------------------------------------------------------------------------
	// Атрибуты строки
	// ----------------------------------------------------------------------------
	virtual wxListItemAttr *OnGetItemAttr(long item) const;

	// ----------------------------------------------------------------------------
	// Картинки в строках
	// ----------------------------------------------------------------------------
	virtual int OnGetItemImage(long item) const;

	// ----------------------------------------------------------------------------
	// и ячейках
	// ----------------------------------------------------------------------------
	virtual int OnGetItemColumnImage(long item, long column) const;

	// ----------------------------------------------------------------------------
	// Конекстное меню
	// ----------------------------------------------------------------------------
//	void OnContextMenu(wxContextMenuEvent& event);
//	void OnAutoUpdate(wxCommandEvent& event);

	// ----------------------------------------------------------------------------
	// Таймер
	// ----------------------------------------------------------------------------
//	void OnTimer(wxTimerEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif // EVENTS_TABLE_H
