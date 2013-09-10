#include "events_table.h"

enum {
	ID_TIMER,
	ID_MENU_AUTOSCROLL,
	ID_MENU_AUTOUPDATE
};

BEGIN_EVENT_TABLE(cEventsTable, wxListCtrl)
//	EVT_CONTEXT_MENU(cEventsTable::OnContextMenu)
//	EVT_MENU(ID_MENU_AUTOUPDATE, cEventsTable::OnAutoUpdate)
	EVT_TIMER(ID_TIMER, cEventsTable::OnTimer)
END_EVENT_TABLE()


#define COL_TIME		0
#define COL_DEV_SN		1
#define COL_REC_ID		2
#define COL_TYPE		3
#define COL_SERVER		4

cEventsTable::cEventsTable(tEventArray *a_data, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator &validator, const wxString &name)
	: wxListCtrl(parent, winid, pos, size, style | wxLC_VIRTUAL, validator, name)
{
	m_events_data = a_data;
	m_Timer = new wxTimer(this, ID_TIMER);
	m_Timer->Start(AUTOUPDATE_INTERVAL);

#if USE_TIMESTAMP_WITH_DATE
	InsertColumn(COL_TIME,   _T("Time"),   wxLIST_FORMAT_LEFT, 150);
#else
	InsertColumn(COL_TIME,   _T("Time"),   wxLIST_FORMAT_LEFT, 60);
#endif
	InsertColumn(COL_DEV_SN, _T("DevSN"),  wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_REC_ID, _T("RecID"),  wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_TYPE,   _T("Type"),   wxLIST_FORMAT_LEFT, 30);
	InsertColumn(COL_SERVER, _T("Server"), wxLIST_FORMAT_LEFT, 30);

	ReInit();

}

cEventsTable::~cEventsTable()
{
	m_Timer->Stop();
	delete m_Timer;
//	delete m_PopupMenu;
}


// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cEventsTable::OnGetItemText(long item, long column) const
{
	if (m_events_data)
	{
		switch (column)
		{
			case COL_TIME:
				return m_events_data->Item(item).time;
			case COL_DEV_SN:
				return wxString::Format("%d", m_events_data->Item(item).dev_sn);
			case COL_REC_ID:
				return wxString::Format("%d", m_events_data->Item(item).record_id);
			case COL_TYPE:
				return wxString::Format("%d", m_events_data->Item(item).rec_type);
			case COL_SERVER:
				return m_events_data->Item(item).server_name;
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
wxListItemAttr *cEventsTable::OnGetItemAttr(long item) const
{
	return NULL;
}

// ----------------------------------------------------------------------------
// Картинки в строках
// ----------------------------------------------------------------------------
int cEventsTable::OnGetItemImage(long item) const
{
	return -1;
}
// ----------------------------------------------------------------------------

int cEventsTable::OnGetItemColumnImage(long item, long column) const
{
	return -1;
}
// ----------------------------------------------------------------------------

void cEventsTable::ReInit()
{
	if (m_events_data)
	{
		SetItemCount(m_events_data->GetCount());
		Refresh();
//		if(m_PopupMenu->IsChecked(ID_MENU_AUTOSCROLL) && GetItemCount())
//			EnsureVisible(GetItemCount() - 1);
	}
}
// ----------------------------------------------------------------------------
//void cEventsTable::OnContextMenu(wxContextMenuEvent& event)
//{
//	PopupMenu(m_PopupMenu);
//	return;
//}
//void cEventsTable::OnAutoUpdate(wxCommandEvent& event)
//{
//	if(event.IsChecked())
//		m_Timer->Start(AUTOUPDATE_INTERVAL);
//	else
//		m_Timer->Stop();
//}
void cEventsTable::OnTimer(wxTimerEvent& event)
{
	if((size_t)GetItemCount() != m_events_data->GetCount())
		ReInit();
}

