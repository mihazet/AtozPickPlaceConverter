#include "comp_type_table.h"

enum {
	ID_TIMER,
	ID_MENU_AUTOSCROLL,
	ID_MENU_AUTOUPDATE
};

BEGIN_EVENT_TABLE(cCompTypeTable, wxListCtrl)
//	EVT_CONTEXT_MENU(cCompTypeTable::OnContextMenu)
//	EVT_MENU(ID_MENU_AUTOUPDATE, cCompTypeTable::OnAutoUpdate)
//	EVT_TIMER(ID_TIMER, cCompTypeTable::OnTimer)
END_EVENT_TABLE()

enum {
	COL_NUM = 0,
	COL_NAME,
	COL_PATTERN,
	COL_PNP_NAME,
	COL_COMP_COUNT,
	COL_ENABLED,
	COL_IS_NEW,
	COL_COUNT
};

cCompTypeTable::cCompTypeTable(tComponentTypeDescr *a_data, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator &validator, const wxString &name)
	: wxListCtrl(parent, winid, pos, size, style | wxLC_VIRTUAL, validator, name)
{
	m_comp_type_data = a_data;
//	m_Timer = new wxTimer(this, ID_TIMER);
//	m_Timer->Start(AUTOUPDATE_INTERVAL);

	InsertColumn(COL_NUM,		_T("#"),	wxLIST_FORMAT_LEFT, 32);
	InsertColumn(COL_NAME,		_T("Name"),	wxLIST_FORMAT_LEFT, 120);
	InsertColumn(COL_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_NAME,	_T("PNP Name"),	wxLIST_FORMAT_LEFT, 110);
	InsertColumn(COL_COMP_COUNT,	_T("Count"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_IS_NEW,	_T("New"),	wxLIST_FORMAT_LEFT, 50);

	ReInit();

}

cCompTypeTable::~cCompTypeTable()
{
//	m_Timer->Stop();
//	delete m_Timer;
//	delete m_PopupMenu;
}


// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cCompTypeTable::OnGetItemText(long item, long column) const
{
	if (m_comp_type_data)
	{
		t_component_type_descr *data = m_comp_type_data->Item(item);
		switch (column)
		{
			case COL_NUM:
				return wxString::Format("%ld", item+1);
			case COL_NAME:
				return data->name;
			case COL_PATTERN:
				return data->pattern;
			case COL_PNP_NAME:
				return data->pnp_name;
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
wxListItemAttr *cCompTypeTable::OnGetItemAttr(long item) const
{
	return NULL;
}

// ----------------------------------------------------------------------------
// Картинки в строках
// ----------------------------------------------------------------------------
int cCompTypeTable::OnGetItemImage(long item) const
{
	return -1;
}
// ----------------------------------------------------------------------------

int cCompTypeTable::OnGetItemColumnImage(long item, long column) const
{
	return -1;
}
// ----------------------------------------------------------------------------

void cCompTypeTable::ReInit()
{
	if (m_comp_type_data)
	{
		SetItemCount(m_comp_type_data->GetCount());
		Refresh();
//		if(m_PopupMenu->IsChecked(ID_MENU_AUTOSCROLL) && GetItemCount())
//			EnsureVisible(GetItemCount() - 1);
	}
}
// ----------------------------------------------------------------------------
//void cCompTypeTable::OnContextMenu(wxContextMenuEvent& event)
//{
//	PopupMenu(m_PopupMenu);
//	return;
//}
//void cCompTypeTable::OnAutoUpdate(wxCommandEvent& event)
//{
//	if(event.IsChecked())
//		m_Timer->Start(AUTOUPDATE_INTERVAL);
//	else
//		m_Timer->Stop();
//}
//void cCompTypeTable::OnTimer(wxTimerEvent& event)
//{
//	if((size_t)GetItemCount() != m_events_data->GetCount())
//		ReInit();
//}
//
