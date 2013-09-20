#include "comp_type_table.h"
#include <wx/fileconf.h>

enum eCompTypeTable {
	COL_NAME = 0,
	COL_PATTERN,
	COL_PNP_NAME,
	COL_COMP_COUNT,
	COL_ENABLED,
	COL_IS_NEW,
	COL_COUNT
};

cCompTypeTable::cCompTypeTable(tComponentTypeDescr *a_data)
	: wxGridTableBase()
{
	m_comp_type_data = a_data;
	m_config = NULL;

	wxGridCellAttrProvider *attrProvider = new wxGridCellAttrProvider;
	SetAttrProvider (attrProvider);

	wxGridCellAttr *ro_attr = new wxGridCellAttr();
	ro_attr->SetReadOnly();
	SetColAttr (ro_attr, COL_NAME);
	SetColAttr (ro_attr, COL_IS_NEW);

	wxGridCellAttr *ro_num_attr = new wxGridCellAttr();
	wxGridCellNumberRenderer *num_rend = new wxGridCellNumberRenderer();
	ro_num_attr->SetRenderer(num_rend);
	ro_num_attr->SetReadOnly();
	SetColAttr (ro_num_attr, COL_COMP_COUNT);

	wxGridCellAttr *bool_attr = new wxGridCellAttr();
	wxGridCellBoolRenderer *bool_rend = new wxGridCellBoolRenderer();
	bool_attr->SetRenderer(bool_rend);
	wxGridCellBoolEditor *bool_edit = new wxGridCellBoolEditor();
	bool_edit->UseStringValues("1", "0");
	bool_attr->SetEditor(bool_edit);
	SetColAttr (bool_attr, COL_ENABLED);

//	InsertColumn(COL_NAME,		_T("Name"),	wxLIST_FORMAT_LEFT, 120);
//	InsertColumn(COL_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 70);
//	InsertColumn(COL_PNP_NAME,	_T("PNP Name"),	wxLIST_FORMAT_LEFT, 110);
//	InsertColumn(COL_COMP_COUNT,	_T("Count"),	wxLIST_FORMAT_LEFT, 50);
//	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
//	InsertColumn(COL_IS_NEW,	_T("New"),	wxLIST_FORMAT_LEFT, 50);
}

cCompTypeTable::~cCompTypeTable()
{
}

int cCompTypeTable::GetNumberRows()
{
	if(NULL == m_comp_type_data)
		return 0;
	return m_comp_type_data->GetCount();
}
int cCompTypeTable::GetNumberCols()
{
	return COL_COUNT;
}

wxString cCompTypeTable::GetColLabelValue( int a_col )
{
	wxString result = wxEmptyString;
	switch (a_col)
	{
		case COL_NAME:
			result = _T("Name");
			break;
		case COL_PATTERN:
			result = _T("Pattern");
			break;
		case COL_PNP_NAME:
			result = _T("Name in PnP");
			break;
		case COL_COMP_COUNT:
			result = _T("Count");
			break;
		case COL_ENABLED:
			result = _T("To OUT");
			break;
		case COL_IS_NEW:
			result = _T("New");
			break;
	}
	return result;
}

// ----------------------------------------------------------------------------
// Текст в ячейках
// ----------------------------------------------------------------------------
wxString cCompTypeTable::GetValue(int a_row, int a_col)
{
	if((NULL == m_comp_type_data) || (a_row >= (int)m_comp_type_data->GetCount()))
		return wxEmptyString;

	wxString result = wxEmptyString;
	t_component_type_descr *data = m_comp_type_data->Item(a_row);
	switch (a_col)
	{
		case COL_NAME:
			result = data->name;
			break;
		case COL_PATTERN:
			result = data->pattern;
			break;
		case COL_PNP_NAME:
			result = data->pnp_name;
			break;
		case COL_COMP_COUNT:
			result = wxString::Format("%zu", data->comp_count);
			break;
		case COL_ENABLED:
			result = wxString::Format("%d", data->enabled);
			break;
		case COL_IS_NEW:
			result = wxString::Format("%d", data->is_new);
			break;
	}
	return result;
}

void cCompTypeTable::SetValue(int a_row, int a_col, const wxString& a_value)
{
	long tmp_long;
	if((NULL == m_config) || (NULL == m_comp_type_data) || (a_row >= (int)m_comp_type_data->GetCount()))
		return;
	if(a_value.IsEmpty())
		return;

	t_component_type_descr *data = m_comp_type_data->Item(a_row);
	wxConfigPathChanger cfg_cd_to(m_config, "/"+data->name+"/");
	switch(a_col)
	{
		case COL_PATTERN:
			data->pattern = a_value;
			m_config->Write("pattern", a_value);
			break;
		case COL_PNP_NAME:
			data->pnp_name = a_value;
			m_config->Write("pnp_name", a_value);
			break;
		case COL_ENABLED:
			a_value.ToLong(&tmp_long);
			data->enabled = tmp_long;
			m_config->Write("enabled", a_value);
			break;
	}
}
