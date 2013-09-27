#include "comp_table.h"

BEGIN_EVENT_TABLE(cCompTable, wxListCtrl)
//	EVT_CONTEXT_MENU(cCompTable::OnContextMenu)
//	EVT_MENU(ID_MENU_AUTOUPDATE, cCompTable::OnAutoUpdate)
//	EVT_TIMER(ID_TIMER, cCompTable::OnTimer)
END_EVENT_TABLE()

enum {
	COL_NUM = 0,
	COL_DESIGNATOR,
	COL_CAD_NAME,
	COL_CAD_PATTERN,
	COL_CAD_VALUE,
	COL_FULL_NAME,
	COL_LAYER,
	COL_CAD_LOCATION_X,
	COL_CAD_LOCATION_Y,
	COL_CAD_ANGLE,
	COL_ENABLED,
	COL_PATTERN,
	COL_PNP_NAME,
	COL_PNP_PACKAGE,
	COL_PNP_FOOTPRINT,
	COL_PNP_LOCATION_X,
	COL_PNP_LOCATION_Y,
	COL_PNP_ANGLE,
	COL_PNP_SUBPCB,
	COL_PNP_ENABLED,
	COL_COUNT
};

cCompTable::cCompTable(tComponentDescr *a_data, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator &validator, const wxString &name)
	: wxListCtrl(parent, winid, pos, size, style | wxLC_VIRTUAL, validator, name)
{
	m_comp_data = a_data;

	m_attr_disabled_comp = new wxListItemAttr;
	m_attr_disabled_comp->SetTextColour(*wxLIGHT_GREY);
	m_attr_data_to_out = new wxListItemAttr;
	m_attr_data_to_out->SetBackgroundColour(*wxGREEN);

	InsertColumn(COL_NUM,		_T("#"),	wxLIST_FORMAT_LEFT, 32);
	InsertColumn(COL_DESIGNATOR,	_T("Des"),	wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_CAD_NAME,	_T("Name"),	wxLIST_FORMAT_LEFT, 110);
	InsertColumn(COL_CAD_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_CAD_VALUE,	_T("Value"),	wxLIST_FORMAT_LEFT, 60);
	InsertColumn(COL_FULL_NAME,	_T("ID"),	wxLIST_FORMAT_LEFT, 120);
	InsertColumn(COL_LAYER,		_T("Layer"),	wxLIST_FORMAT_LEFT, 40);
	InsertColumn(COL_CAD_LOCATION_X,_T("X"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_CAD_LOCATION_Y,_T("Y"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_CAD_ANGLE,	_T("Angle"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 110);
	InsertColumn(COL_PNP_NAME,	_T("PNP Comp"),	wxLIST_FORMAT_LEFT, 110);
	InsertColumn(COL_PNP_PACKAGE,	_T("PNP Pack"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_FOOTPRINT,	_T("PNP Foot"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_LOCATION_X,_T("PNP X"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_LOCATION_Y,_T("PNP Y"),	wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_ANGLE,	_T("PNP Angle"),wxLIST_FORMAT_LEFT, 70);
	InsertColumn(COL_PNP_SUBPCB,	_T("PNP pcb"),	wxLIST_FORMAT_LEFT, 50);
	InsertColumn(COL_PNP_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);

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
		t_component_descr *data = &(m_comp_data->Item(item));
		switch (column)
		{
			case COL_NUM:
				return wxString::Format("%ld", item+1);
			case COL_DESIGNATOR:
				return data->designator;
			case COL_CAD_NAME:
				return data->cad_name;
			case COL_CAD_PATTERN:
				return data->cad_pattern;
			case COL_CAD_VALUE:
				return data->cad_value;
			case COL_FULL_NAME:
				return data->full_name;
			case COL_LAYER:
				return data->layer;
			case COL_CAD_LOCATION_X:
				return wxString::Format("%.3f", data->cad_location_x);
			case COL_CAD_LOCATION_Y:
				return wxString::Format("%.3f", data->cad_location_y);
			case COL_CAD_ANGLE:
				return wxString::Format("%.1f", data->cad_angle);
			case COL_ENABLED:
				return wxString::Format("%d", data->enabled);
			case COL_PATTERN:
				return data->pattern;
			case COL_PNP_NAME:
				return data->pnp_name;
			case COL_PNP_PACKAGE:
				return data->pnp_package;
			case COL_PNP_FOOTPRINT:
				return data->pnp_footprint;
			case COL_PNP_LOCATION_X:
				return wxString::Format("%.3f", data->pnp_location_x);
			case COL_PNP_LOCATION_Y:
				return wxString::Format("%.3f", data->pnp_location_y);
			case COL_PNP_ANGLE:
				return wxString::Format("%.1f", data->pnp_angle);
			case COL_PNP_SUBPCB:
				return wxString::Format("%zu", data->pnp_subpcb_index);
			case COL_PNP_ENABLED:
				return wxString::Format("%d", data->pnp_enabled);
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
	if(NULL == m_comp_data)
		return NULL;

	t_component_descr *data = &(m_comp_data->Item(item));
	wxListItemAttr *result = NULL;

	if(!(data->enabled && data->pnp_enabled))
	{
		result = m_attr_disabled_comp;
	}
	return result;
}
wxListItemAttr *cCompTable::OnGetItemColumnAttr(long item, long column) const
{
	if(NULL == m_comp_data)
		return NULL;

	wxListItemAttr *result = OnGetItemAttr(item);

	if(NULL == result)
	{
		switch (column)
		{
			case COL_DESIGNATOR:
			case COL_LAYER:
			case COL_PNP_NAME:
			case COL_PNP_FOOTPRINT:
			case COL_PNP_LOCATION_X:
			case COL_PNP_LOCATION_Y:
			case COL_PNP_ANGLE:
				result = m_attr_data_to_out;
			default:
				result = NULL;
		}
	}
	return result;
}

void cCompTable::ReInit()
{
	if (NULL == m_comp_data)
		return;

	SetItemCount(m_comp_data->GetCount());
	Refresh();
//	if(m_PopupMenu->IsChecked(ID_MENU_AUTOSCROLL) && GetItemCount())
//		EnsureVisible(GetItemCount() - 1);
}
