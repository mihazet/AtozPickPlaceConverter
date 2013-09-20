#include "fid_mark_table.h"
#include <wx/fileconf.h>

enum eFidMarkTable {
	COL_DESIGNATOR = 0,
	COL_NAME,
	COL_SUBPCB_INDEX,
	COL_USE_ON_SUBPCB,
	COL_USE_GLOBAL,
	COL_LOCATION_X,
	COL_LOCATION_Y,
	COL_LOCAL_FOR,
	COL_COUNT
};

cCompTypeTable::cFidMarkTable(tComponentDescr *a_comps, tFidMarkDescr *a_fidmarks)
	: wxGridTableBase()
{
	m_component_data = a_data;
	m_fid_mark_data = a_data;
	m_config = NULL;

	wxGridCellAttrProvider *attrProvider = new wxGridCellAttrProvider;
	SetAttrProvider (attrProvider);

	wxGridCellAttr *ro_attr = new wxGridCellAttr();
	ro_attr->SetReadOnly();
	SetColAttr (ro_attr, COL_DESIGNATOR);
	SetColAttr (ro_attr, COL_NAME);
	SetColAttr (ro_attr, COL_SUBPCB_INDEX);

	wxGridCellAttr *ro_float_attr = new wxGridCellAttr();
	wxGridCellFloatRenderer *float_rend = new wxGridCellFloatRenderer(5, 3);
	ro_float_attr->SetRenderer(float_rend);
	ro_float_attr->SetReadOnly();
	SetColAttr (ro_float_attr, COL_LOCATION_X);
	SetColAttr (ro_float_attr, COL_LOCATION_Y);

	m_array_on_subpcb.Add("Not use");
	m_array_on_subpcb.Add("Mark 1");
	m_array_on_subpcb.Add("Mark 2");
	m_array_on_subpcb.Add("Mark 3");
	m_array_on_subpcb.Add("Local");
	wxGridCellAttr *choise_on_subpcb_attr = new wxGridCellAttr();
	wxGridCellChoiceEditor *choise_on_subpcb_edit = new wxGridCellChoiceEditor(m_array_on_subpcb);
	choise_on_subpcb_attr->SetEditor(choise_on_subpcb_edit);
	SetColAttr (choise_on_subpcb_attr, COL_USE_ON_SUBPCB);

	m_array_global.Add("Not use");
	m_array_global.Add("Mark 1");
	m_array_global.Add("Mark 2");
	m_array_global.Add("Mark 3");
	wxGridCellAttr *choise_global_attr = new wxGridCellAttr();
	wxGridCellChoiceEditor *choise_global_edit = new wxGridCellChoiceEditor(m_array_global);
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
	t_component_descr *data_comp = m_component_data->Item(data_fidmark->component_index);
	switch (a_col)
	{
		case COL_DESIGNATOR:
			result = data_comp->designator;
			break;
		case COL_NAME:
			result = data_comp->pnp_name;
			break;
		case COL_SUBPCB_INDEX:
			result = wxString::Format("%zu", data_comp->pnp_subpcb_index);
			break;
		case COL_USE_ON_SUBPCB:
			result = m_array_on_subpcb[m_fid_mark_data->usage_type];
			break;
		case COL_USE_GLOBAL:
			result = m_array_on_subpcb[m_fid_mark_data->usage_as_global];
			break;
		case COL_LOCATION_X:
			result = wxString::Format("%f", m_fid_mark_data->pnp_location_x);
			break;
		case COL_LOCATION_Y:
			result = wxString::Format("%f", m_fid_mark_data->pnp_location_y);
			break;
		case COL_LOCAL_FOR:
			for(size_t local_for_count = m_fid_mark_data->local_for_comps.GetCount(), size_t index = 0; index < local_for_count; index++)
			{
				result += m_fid_mark_data->local_for_comps[index] + ";";
			}
			result = result.BeforeLast(";");//FIXME не самый красивый метод убрать последний символ
			break;
	}
	return result;
}

void cFidMarkTable::SetValue(int a_row, int a_col, const wxString& a_value)
{
	long tmp_long;
	bool component_found = false;
	if((NULL == m_config) || (NULL == m_component_data) || (NULL == m_fid_mark_data) || (a_row >= (int)m_fid_mark_data->GetCount()))
		return;
	if((COL_LOCAL_FOR != a_col) && a_value.IsEmpty())
		return;

	wxString component;
	t_fid_mark_descr *data_fidmark = m_fid_mark_data->Item(a_row);
	t_component_descr *data_comp = m_component_data->Item(data_fidmark->component_index);
//	wxConfigPathChanger cfg_cd_to(m_config, "/"+data->name+"/");
	switch(a_col)
	{
		case COL_USE_ON_SUBPCB:
//			result = m_array_on_subpcb[m_fid_mark_data->usage_type];
			break;
		case COL_USE_GLOBAL:
//			result = m_array_on_subpcb[m_fid_mark_data->usage_as_global];
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
}
