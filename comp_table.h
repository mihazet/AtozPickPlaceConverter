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
	wxListItemAttr		*m_attr_disabled_comp;
	wxListItemAttr		*m_attr_data_to_out;


	// Переопределяем виртуальные функции

	// ----------------------------------------------------------------------------
	// Текст в ячейках
	// ----------------------------------------------------------------------------
	virtual wxString OnGetItemText(long item, long column) const;

	// ----------------------------------------------------------------------------
	// Атрибуты строки
	// ----------------------------------------------------------------------------
	virtual wxListItemAttr *OnGetItemAttr(long item) const;
	virtual wxListItemAttr *OnGetItemColumnAttr(long item, long column) const;
	DECLARE_EVENT_TABLE()
};

#endif // EVENTS_TABLE_H
