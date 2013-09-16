#include "pattern_table.h"
#include <wx/fileconf.h>


enum ePatternTable {
	COL_PATTERN = 0,
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

cPatternTable::cPatternTable(tPatternDescr *a_data)
	: wxGridTableBase()
{
	m_pattern_data = a_data;
	m_config = NULL;

	wxGridCellAttrProvider *attrProvider = new wxGridCellAttrProvider;
	SetAttrProvider (attrProvider);

	wxGridCellAttr *ro_attr = new wxGridCellAttr();
	ro_attr->SetReadOnly();
	SetColAttr (ro_attr, COL_PATTERN);
	SetColAttr (ro_attr, COL_IS_NEW);

	wxGridCellAttr *ro_num_attr = new wxGridCellAttr();
	wxGridCellNumberRenderer *num_rend = new wxGridCellNumberRenderer();
	ro_num_attr->SetRenderer(num_rend);
	ro_num_attr->SetReadOnly();
	SetColAttr (ro_num_attr, COL_COMP_COUNT);

	wxGridCellAttr *float1_attr = new wxGridCellAttr();
	wxGridCellFloatRenderer *float1_rend = new wxGridCellFloatRenderer(3, 1);
	float1_attr->SetRenderer(float1_rend);
	wxGridCellFloatEditor *float1_edit = new wxGridCellFloatEditor(3, 1);
	float1_attr->SetEditor(float1_edit);
	SetColAttr (float1_attr, COL_ANGLE);

	wxGridCellAttr *float3_attr = new wxGridCellAttr();
	wxGridCellFloatRenderer *float3_rend = new wxGridCellFloatRenderer(5, 3);
	float3_attr->SetRenderer(float3_rend);
	wxGridCellFloatEditor *float3_edit = new wxGridCellFloatEditor(5, 3);
	float3_attr->SetEditor(float3_edit);
	SetColAttr (float3_attr, COL_OFFSET_X);
	SetColAttr (float3_attr, COL_OFFSET_Y);

	wxGridCellAttr *bool_attr = new wxGridCellAttr();
	wxGridCellBoolRenderer *bool_rend = new wxGridCellBoolRenderer();
	bool_attr->SetRenderer(bool_rend);
	wxGridCellBoolEditor *bool_edit = new wxGridCellBoolEditor();
	bool_edit->UseStringValues("1", "0");
	bool_attr->SetEditor(bool_edit);
	SetColAttr (bool_attr, COL_ENABLED);

//	InsertColumn(COL_PATTERN,	_T("Patt Name"),wxLIST_FORMAT_LEFT, 120);
//	InsertColumn(COL_PNP_PACKAGE,	_T("Package"),	wxLIST_FORMAT_LEFT, 120);
//	InsertColumn(COL_PNP_FOOTPRINT,	_T("Footprint"),wxLIST_FORMAT_LEFT, 120);
//	InsertColumn(COL_OFFSET_X,	_T("X"),	wxLIST_FORMAT_LEFT, 70);
//	InsertColumn(COL_OFFSET_Y,	_T("Y"),	wxLIST_FORMAT_LEFT, 70);
//	InsertColumn(COL_ANGLE,		_T("Angle"),	wxLIST_FORMAT_LEFT, 70);
//	InsertColumn(COL_COMP_COUNT,	_T("Count"),	wxLIST_FORMAT_LEFT, 50);
//	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
//	InsertColumn(COL_IS_NEW,	_T("New"),	wxLIST_FORMAT_LEFT, 50);
}

cPatternTable::~cPatternTable()
{
}

int cPatternTable::GetNumberRows()
{
	if(NULL == m_pattern_data)
		return 0;
	return m_pattern_data->GetCount();
}
int cPatternTable::GetNumberCols()
{
	return COL_COUNT;
}

wxString cPatternTable::GetColLabelValue( int a_col )
{
	wxString result = wxEmptyString;
	switch (a_col)
	{
		case COL_PATTERN:
			result = _T("Patt Name");
			break;
		case COL_PNP_PACKAGE:
			result = _T("Package");
			break;
		case COL_PNP_FOOTPRINT:
			result = _T("Footprint");
			break;
		case COL_OFFSET_X:
			result = _T("X");
			break;
		case COL_OFFSET_Y:
			result = _T("Y");
			break;
		case COL_ANGLE:
			result = _T("Angle");
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
wxString cPatternTable::GetValue(int a_row, int a_col)
{
	if((NULL == m_pattern_data) || (a_row >= (int)m_pattern_data->GetCount()))
		return wxEmptyString;

	wxString result = wxEmptyString;
	t_pattern_descr *data = m_pattern_data->Item(a_row);
	switch (a_col)
	{
		case COL_PATTERN:
			result = data->pattern;
			break;
		case COL_PNP_PACKAGE:
			result = data->pnp_package;
			break;
		case COL_PNP_FOOTPRINT:
			result = data->pnp_footprint;
			break;
		case COL_OFFSET_X:
			result = wxString::Format("%f", data->offset_x);
			break;
		case COL_OFFSET_Y:
			result = wxString::Format("%f", data->offset_y);
			break;
		case COL_ANGLE:
			result = wxString::Format("%f", data->angle);
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

void cPatternTable::SetValue(int a_row, int a_col, const wxString& a_value)
{
	double tmp_double;
	long tmp_long;
	if((NULL == m_config) || (NULL == m_pattern_data) || (a_row >= (int)m_pattern_data->GetCount()))
		return;
	if(a_value.IsEmpty())
		return;

	t_pattern_descr *data = m_pattern_data->Item(a_row);
	wxConfigPathChanger cfg_cd_to(m_config, "/"+data->pattern+"/");
	switch(a_col)
	{
		case COL_PNP_PACKAGE:
			data->pnp_package = a_value;
			m_config->Write("pnp_package", a_value);
			break;
		case COL_PNP_FOOTPRINT:
			data->pnp_footprint = a_value;
			m_config->Write("pnp_footprint", a_value);
			break;
		case COL_OFFSET_X:
			a_value.ToDouble(&tmp_double);
			data->offset_x = tmp_double;
			m_config->Write("offset_x", a_value);
			break;
		case COL_OFFSET_Y:
			a_value.ToDouble(&tmp_double);
			data->offset_y = tmp_double;
			m_config->Write("offset_y", a_value);
			break;
		case COL_ANGLE:
			a_value.ToDouble(&tmp_double);
			data->angle = tmp_double;
			m_config->Write("angle", a_value);
			break;
		case COL_ENABLED:
			a_value.ToLong(&tmp_long);
			data->enabled = tmp_long;
			m_config->Write("enabled", a_value);
			break;
	}
}