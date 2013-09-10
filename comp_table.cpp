#include "comp_table.h"

enum {
	ID_TIMER,
	ID_MENU_AUTOSCROLL,
	ID_MENU_AUTOUPDATE
};

BEGIN_EVENT_TABLE(cCompTable, wxListCtrl)
//	EVT_CONTEXT_MENU(cCompTable::OnContextMenu)
//	EVT_MENU(ID_MENU_AUTOUPDATE, cCompTable::OnAutoUpdate)
//	EVT_TIMER(ID_TIMER, cCompTable::OnTimer)
END_EVENT_TABLE()

enum {
	COL_NUM = 0,
	COL_DESIGNATOR,
	COL_PATTERN,
	COL_SRC_NAME,
	COL_NAME,
	COL_VALUE,
	COL_LAYER,
	COL_LOCATION_X,
	COL_LOCATION_Y,
	COL_ANGLE,
	COL_ENABLED,
	COL_COUNT
};

cCompTable::cCompTable(tComponentDescr *a_data, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator &validator, const wxString &name)
	: wxListCtrl(parent, winid, pos, size, style | wxLC_VIRTUAL, validator, name)
{
	m_comp_data = a_data;
//	m_Timer = new wxTimer(this, ID_TIMER);
//	m_Timer->Start(AUTOUPDATE_INTERVAL);

	InsertColumn(COL_NUM,		_T("#"),	wxLIST_FORMAT_LEFT, 32);
	InsertColumn(COL_DESIGNATOR,	_T("Des"),	wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_SRC_NAME,	_T("Name"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_NAME,		_T("New Name"),	wxLIST_FORMAT_LEFT, 110);
	InsertColumn(COL_VALUE,		_T("Value"),	wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_LAYER,		_T("Layer"),	wxLIST_FORMAT_LEFT, 40);
	InsertColumn(COL_LOCATION_X,	_T("X"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_LOCATION_Y,	_T("Y"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_ANGLE,		_T("Angle"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);

	ReInit();

}

cCompTable::~cCompTable()
{
//	m_Timer->Stop();
//	delete m_Timer;
//	delete m_PopupMenu;
}


// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cCompTable::OnGetItemText(long item, long column) const
{
	if (m_comp_data)
	{
		switch (column)
		{
			case COL_NUM:
				return wxString::Format("%ld", item+1);
			case COL_DESIGNATOR:
				return m_comp_data->Item(item).designator;
			case COL_PATTERN:
				return m_comp_data->Item(item).pattern;
			case COL_SRC_NAME:
				return m_comp_data->Item(item).src_name;
			case COL_NAME:
				return m_comp_data->Item(item).name;
			case COL_VALUE:
				return m_comp_data->Item(item).value;
			case COL_LAYER:
				return m_comp_data->Item(item).layer;
			case COL_LOCATION_X:
				return wxString::Format("%.3f", m_comp_data->Item(item).location_x);
			case COL_LOCATION_Y:
				return wxString::Format("%.3f", m_comp_data->Item(item).location_y);
			case COL_ANGLE:
				return wxString::Format("%.1f", m_comp_data->Item(item).angle);
			case COL_ENABLED:
				return wxString::Format("%d", m_comp_data->Item(item).enabled);;
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
wxListItemAttr *cCompTable::OnGetItemAttr(long item) const
{
	return NULL;
}

// ----------------------------------------------------------------------------
// Картинки в строках
// ----------------------------------------------------------------------------
int cCompTable::OnGetItemImage(long item) const
{
	return -1;
}
// ----------------------------------------------------------------------------

int cCompTable::OnGetItemColumnImage(long item, long column) const
{
	return -1;
}
// ----------------------------------------------------------------------------

void cCompTable::ReInit()
{
	if (m_comp_data)
	{
		SetItemCount(m_comp_data->GetCount());
		Refresh();
//		if(m_PopupMenu->IsChecked(ID_MENU_AUTOSCROLL) && GetItemCount())
//			EnsureVisible(GetItemCount() - 1);
	}
}
// ----------------------------------------------------------------------------
//void cCompTable::OnContextMenu(wxContextMenuEvent& event)
//{
//	PopupMenu(m_PopupMenu);
//	return;
//}
//void cCompTable::OnAutoUpdate(wxCommandEvent& event)
//{
//	if(event.IsChecked())
//		m_Timer->Start(AUTOUPDATE_INTERVAL);
//	else
//		m_Timer->Stop();
//}
//void cCompTable::OnTimer(wxTimerEvent& event)
//{
//	if((size_t)GetItemCount() != m_events_data->GetCount())
//		ReInit();
//}
//
