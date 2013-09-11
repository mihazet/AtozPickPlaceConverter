#include "pattern_table.h"

enum {
	ID_TIMER,
	ID_MENU_AUTOSCROLL,
	ID_MENU_AUTOUPDATE
};

BEGIN_EVENT_TABLE(cPatternTable, wxListCtrl)
//	EVT_CONTEXT_MENU(cPatternTable::OnContextMenu)
//	EVT_MENU(ID_MENU_AUTOUPDATE, cPatternTable::OnAutoUpdate)
//	EVT_TIMER(ID_TIMER, cPatternTable::OnTimer)
END_EVENT_TABLE()

enum {
	COL_NUM = 0,
	COL_PATTERN,
	COL_PNP_PACKAGE,
	COL_PNP_FOOTPRINT,
	COL_OFFSET_X,
	COL_OFFSET_Y,
	COL_ANGLE,
	COL_COMP_COUNT,
	COL_ENABLED,
	COL_IS_NEW,
	COL_COUNT
};

cPatternTable::cPatternTable(tPatternDescr *a_data, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator &validator, const wxString &name)
	: wxListCtrl(parent, winid, pos, size, style | wxLC_VIRTUAL, validator, name)
{
	m_pattern_data = a_data;
//	m_Timer = new wxTimer(this, ID_TIMER);
//	m_Timer->Start(AUTOUPDATE_INTERVAL);

	InsertColumn(COL_NUM,		_T("#"),	wxLIST_FORMAT_LEFT, 32);
	InsertColumn(COL_PATTERN,	_T("Patt Name"),wxLIST_FORMAT_LEFT, 120);
	InsertColumn(COL_PNP_PACKAGE,	_T("Package"),	wxLIST_FORMAT_LEFT, 120);
	InsertColumn(COL_PNP_FOOTPRINT,	_T("Footprint"),wxLIST_FORMAT_LEFT, 120);
	InsertColumn(COL_OFFSET_X,	_T("X"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_OFFSET_Y,	_T("Y"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_ANGLE,		_T("Angle"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_COMP_COUNT,	_T("Count"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_IS_NEW,	_T("New"),	wxLIST_FORMAT_LEFT, 50);

	ReInit();

}

cPatternTable::~cPatternTable()
{
//	m_Timer->Stop();
//	delete m_Timer;
//	delete m_PopupMenu;
}


// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cPatternTable::OnGetItemText(long item, long column) const
{
	if (m_pattern_data)
	{
		t_pattern_descr *data = m_pattern_data->Item(item);
		switch (column)
		{
			case COL_NUM:
				return wxString::Format("%ld", item+1);
			case COL_PATTERN:
				return data->pattern;
			case COL_PNP_PACKAGE:
				return data->pnp_package;
			case COL_PNP_FOOTPRINT:
				return data->pnp_footprint;
			case COL_OFFSET_X:
				return wxString::Format("%.3f", data->offset_x);
			case COL_OFFSET_Y:
				return wxString::Format("%.3f", data->offset_y);
			case COL_ANGLE:
				return wxString::Format("%.1f", data->angle);
			case COL_COMP_COUNT:
				return wxString::Format("%zu", data->comp_count);;
			case COL_ENABLED:
				return wxString::Format("%d", data->enabled);;
			case COL_IS_NEW:
				return wxString::Format("%d", data->is_new);;
			default:
				return wxEmptyString;
		}
	} else {
		return wxEmptyString;
	}
}

// ----------------------------------------------------------------------------
// Атрибуты строки
// ----------------------------------------------------------------------------
wxListItemAttr *cPatternTable::OnGetItemAttr(long item) const
{
	return NULL;
}

// ----------------------------------------------------------------------------
// Картинки в строках
// ----------------------------------------------------------------------------
int cPatternTable::OnGetItemImage(long item) const
{
	return -1;
}
// ----------------------------------------------------------------------------

int cPatternTable::OnGetItemColumnImage(long item, long column) const
{
	return -1;
}
// ----------------------------------------------------------------------------

void cPatternTable::ReInit()
{
	if (m_pattern_data)
	{
		SetItemCount(m_pattern_data->GetCount());
		Refresh();
//		if(m_PopupMenu->IsChecked(ID_MENU_AUTOSCROLL) && GetItemCount())
//			EnsureVisible(GetItemCount() - 1);
	}
}
// ----------------------------------------------------------------------------
//void cPatternTable::OnContextMenu(wxContextMenuEvent& event)
//{
//	PopupMenu(m_PopupMenu);
//	return;
//}
//void cPatternTable::OnAutoUpdate(wxCommandEvent& event)
//{
//	if(event.IsChecked())
//		m_Timer->Start(AUTOUPDATE_INTERVAL);
//	else
//		m_Timer->Stop();
//}
//void cPatternTable::OnTimer(wxTimerEvent& event)
//{
//	if((size_t)GetItemCount() != m_events_data->GetCount())
//		ReInit();
//}
//
