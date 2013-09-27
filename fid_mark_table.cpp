#include "fid_mark_table.h"
#include "PnP_convMain.h"
#include <wx/fileconf.h>

enum eFidMarkTable {
	COL_DESIGNATOR = 0,
	COL_NAME,
	COL_LAYER,
	COL_SUBPCB_INDEX,
	COL_USE_ON_SUBPCB,
	COL_USE_GLOBAL,
	COL_LOCATION_X,
	COL_LOCATION_Y,
	COL_LOCAL_FOR,
	COL_COUNT
};

cFidMarkTable::cFidMarkTable(tComponentDescr *a_comps, tFidMarkDescr *a_fidmarks, PnP_convFrame *a_data_ctrl)
	: wxGridTableBase()
{
	m_component_data = a_comps;
	m_fid_mark_data = a_fidmarks;
	m_main_data_controller = a_data_ctrl;

	wxGridCellAttrProvider *attrProvider = new wxGridCellAttrProvider;
	SetAttrProvider (attrProvider);

	wxGridCellAttr *ro_attr = new wxGridCellAttr();
	ro_attr->SetReadOnly();
	SetColAttr (ro_attr, COL_DESIGNATOR);
	SetColAttr (ro_attr, COL_NAME);
	SetColAttr (ro_attr, COL_LAYER);
	SetColAttr (ro_attr, COL_SUBPCB_INDEX);

	wxGridCellAttr *ro_float_attr = new wxGridCellAttr();
	wxGridCellFloatRenderer *float_rend = new wxGridCellFloatRenderer(5, 3);
	ro_float_attr->SetRenderer(float_rend);
	ro_float_attr->SetReadOnly();
	SetColAttr (ro_float_attr, COL_LOCATION_X);
	SetColAttr (ro_float_attr, COL_LOCATION_Y);

	wxGridCellAttr *choise_on_subpcb_attr = new wxGridCellAttr();
	wxGridCellChoiceEditor *choise_on_subpcb_edit = new wxGridCellChoiceEditor(G_array_on_subpcb);
	choise_on_subpcb_attr->SetEditor(choise_on_subpcb_edit);
	SetColAttr (choise_on_subpcb_attr, COL_USE_ON_SUBPCB);

	wxGridCellAttr *choise_global_attr = new wxGridCellAttr();
	wxGridCellChoiceEditor *choise_global_edit = new wxGridCellChoiceEditor(G_array_global);
	choise_global_attr->SetEditor(choise_global_edit);
	SetColAttr (choise_global_attr, COL_USE_GLOBAL);
}

cFidMarkTable::~cFidMarkTable()
{
}

int cFidMarkTable::GetNumberRows()
{
	if(NULL == m_fid_mark_data)
		return 0;
	return m_fid_mark_data->GetCount();
}
int cFidMarkTable::GetNumberCols()
{
	return COL_COUNT;
}

wxString cFidMarkTable::GetColLabelValue( int a_col )
{
	wxString result = wxEmptyString;
	switch (a_col)
	{
		case COL_DESIGNATOR:
			result = _T("Designator");
			break;
		case COL_NAME:
			result = _T("Name");
			break;
		case COL_LAYER:
			result = _T("Layer");
			break;
		case COL_SUBPCB_INDEX:
			result = _T("SubPCB");
			break;
		case COL_USE_ON_SUBPCB:
			result = _T("Use as (on subPCB)");
			break;
		case COL_USE_GLOBAL:
			result = _T("Use as (global)");
			break;
		case COL_LOCATION_X:
			result = _T("X");
			break;
		case COL_LOCATION_Y:
			result = _T("Y");
			break;
		case COL_LOCAL_FOR:
			result = _T("Local for components");
			break;
	}
	return result;
}

// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cFidMarkTable::GetValue(int a_row, int a_col)
{
	if((NULL == m_component_data) || (NULL == m_fid_mark_data) || (a_row >= (int)m_fid_mark_data->GetCount()))
		return wxEmptyString;

	;
	wxString result = wxEmptyString;
	t_fid_mark_descr *data_fidmark = m_fid_mark_data->Item(a_row);
	t_component_descr *data_comp = &m_component_data->Item(data_fidmark->component_index);
//	size_t local_for_count;
	switch (a_col)
	{
		case COL_DESIGNATOR:
			result = data_comp->designator;
			break;
		case COL_NAME:
			result = data_comp->pnp_name;
			break;
		case COL_LAYER:
			result = data_comp->layer;
			break;
		case COL_SUBPCB_INDEX:
			result = wxString::Format("%d", data_comp->pnp_subpcb_index);
			break;
		case COL_USE_ON_SUBPCB:
			result = G_array_on_subpcb[data_fidmark->usage_type];
			break;
		case COL_USE_GLOBAL:
			result = G_array_global[data_fidmark->usage_as_global];
			break;
		case COL_LOCATION_X:
			result = wxString::Format("%f", data_comp->pnp_location_x);
			break;
		case COL_LOCATION_Y:
			result = wxString::Format("%f", data_comp->pnp_location_y);
			break;
		case COL_LOCAL_FOR:
//			local_for_count = data_fidmark->local_for_comps.GetCount();
//			for(size_t index = 0; index < local_for_count; index++)
//			{
//				result += data_fidmark->local_for_comps[index] + ";";
//			}
//			result = result.BeforeLast(';');//FIXME не самый красивый метод убрать последний символ
			break;
	}
	return result;
}

void cFidMarkTable::SetValue(int a_row, int a_col, const wxString& a_value)
{
	if((NULL == m_main_data_controller) || (NULL == m_component_data) || (NULL == m_fid_mark_data) || (a_row >= (int)m_fid_mark_data->GetCount()))
		return;
	if((COL_LOCAL_FOR != a_col) && a_value.IsEmpty())
		return;

	t_fid_mark_descr *data_fidmark = m_fid_mark_data->Item(a_row);
	t_component_descr *data_comp = &m_component_data->Item(data_fidmark->component_index);
	long tmp_long;
	bool single_board = m_main_data_controller->IsSingleBoard();
//	wxString component;
//	bool component_found = false;
	switch(a_col)
	{
		case COL_USE_ON_SUBPCB:
			tmp_long = G_array_on_subpcb.Index(a_value, false);
			if(wxNOT_FOUND == tmp_long)
				tmp_long = FID_MARK_USE_UNKNOWN;
			if((FID_MARK_USE_FM1 == tmp_long)||(FID_MARK_USE_FM2 == tmp_long)||(FID_MARK_USE_FM3 == tmp_long))
			{
				for(int index = m_fid_mark_data->GetCount()-1; index >= 0; index--) //проверка, есть ли уже репер с таким номером
				{
					t_fid_mark_descr *fm_descr = m_fid_mark_data->Item(index);
					t_component_descr *fm_comp = &m_component_data->Item(fm_descr->component_index);
					if((fm_comp->layer != data_comp->layer) || (fm_comp->pnp_subpcb_index != data_comp->pnp_subpcb_index))
						continue;
					if(tmp_long == fm_descr->usage_type) //если есть, то меняем местами
					{
						fm_descr->usage_type = data_fidmark->usage_type;
						if(single_board)
							fm_descr->usage_as_global = data_fidmark->usage_as_global;
						break;
					}
				}
			}
			data_fidmark->usage_type = tmp_long;
			if(single_board)
				data_fidmark->usage_as_global = tmp_long;
			break;
		case COL_USE_GLOBAL:
			tmp_long = G_array_global.Index(a_value, false);
			if(wxNOT_FOUND == tmp_long)
				tmp_long = FID_MARK_USE_UNKNOWN;
			if((FID_MARK_USE_FM1 == tmp_long)||(FID_MARK_USE_FM2 == tmp_long)||(FID_MARK_USE_FM3 == tmp_long))
			{
				for(int index = m_fid_mark_data->GetCount()-1; index >= 0; index--) //проверка, есть ли уже репер с таким номером
				{
					t_fid_mark_descr *fm_descr = m_fid_mark_data->Item(index);
					t_component_descr *fm_comp = &m_component_data->Item(fm_descr->component_index);
					if(fm_comp->layer != data_comp->layer)
						continue;
					if(tmp_long == fm_descr->usage_as_global) //если есть, то меняем местами
					{
						fm_descr->usage_as_global = data_fidmark->usage_as_global;
						if(single_board)
							fm_descr->usage_type = data_fidmark->usage_type;
						break;
					}
				}
			}
			data_fidmark->usage_as_global = (wxNOT_FOUND == tmp_long)?FID_MARK_USE_UNKNOWN:tmp_long;
			if(single_board)
				data_fidmark->usage_type = tmp_long;
			break;
		case COL_LOCAL_FOR:
//			if(a_value.IsEmpty())
//				break;
//			component = a_value.BeforeFirst(';');
//			component = component.Trim(true).Trim(false);
//			for(size_t index = m_component_data->GetCount()-1; index >= 0; index--)
//			{
//				if(m_component_data->Item(index)->designator == component)
//				{
//					component_found = true;
//					break;
//				}
//			}
			break;
	}
	m_main_data_controller->SaveProjectInfo();
	m_main_data_controller->Refresh();
}
