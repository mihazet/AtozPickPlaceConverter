#include "pattern_table.h"
#include "project.h"

#include <algorithm>

enum
{
	COL_PATTERN = 0,
	COL_PNP_PACKAGE,
	COL_ADD_TO_NAME,
	COL_PNP_FOOTPRINT,
	COL_OFFSET_X,
	COL_OFFSET_Y,
	COL_ANGLE,
	COL_COMP_COUNT,
	COL_ENABLED,
	COL_IS_NEW,
	COUNT_COLS,
};

PatternTable::PatternTable(Project *project)
	: wxGridTableBase()
	, m_project(project)
	, m_pattern(project->GetPattern())
{
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
	SetColAttr (bool_attr, COL_ADD_TO_NAME);

}

PatternTable::~PatternTable()
{
}

int PatternTable::GetNumberRows()
{
	return m_pattern.size();
}

int PatternTable::GetNumberCols()
{
	return COUNT_COLS;
}

wxString PatternTable::GetColLabelValue( int col )
{
	wxString result = wxEmptyString;
	switch (col)
	{
		case COL_PATTERN:
			result = "Pattern";
			break;
		case COL_PNP_PACKAGE:
			result = "Package";
			break;
		case COL_ADD_TO_NAME:
			result = "Include to name";
			break;
		case COL_PNP_FOOTPRINT:
			result = "Footprint";
			break;
		case COL_OFFSET_X:
			result = "X";
			break;
		case COL_OFFSET_Y:
			result = "Y";
			break;
		case COL_ANGLE:
			result = "Angle";
			break;
		case COL_COMP_COUNT:
			result = "Count";
			break;
		case COL_ENABLED:
			result = "To OUT";
			break;
		case COL_IS_NEW:
			result = "New";
			break;
	}
	return result;
}

wxString PatternTable::GetValue( int row, int col )
{
	wxString result = wxEmptyString;
	Pattern& data = m_pattern[row];
	switch (col)
	{
		case COL_PATTERN:
			result = data.pattern;
			break;
		case COL_PNP_PACKAGE:
			result = data.pnp_package;
			break;
		case COL_ADD_TO_NAME:
			result = wxString::Format("%d", data.add_pack_to_name);
			break;
		case COL_PNP_FOOTPRINT:
			result = data.pnp_footprint;
			break;
		case COL_OFFSET_X:
			result = wxString::Format("%f", data.offset_x);
			break;
		case COL_OFFSET_Y:
			result = wxString::Format("%f", data.offset_y);
			break;
		case COL_ANGLE:
			result = wxString::Format("%f", data.angle);
			break;
		case COL_COMP_COUNT:
			result = wxString::Format("%d", data.comp_count);
			break;
		case COL_ENABLED:
			result = wxString::Format("%d", data.enabled);
			break;
		case COL_IS_NEW:
			result = wxString::Format("%d", data.is_new);
			break;
	}
	return result;
}

void PatternTable::SetValue( int row, int col, const wxString& value )
{
	double tmp_double;
	long tmp_long;

	if (value.IsEmpty())
		return;

	Pattern& data = m_pattern[row];
	switch (col)
	{
		case COL_PNP_PACKAGE:
			data.pnp_package = value;
			break;
		case COL_ADD_TO_NAME:
			value.ToLong(&tmp_long);
			data.add_pack_to_name = tmp_long;
			break;
		case COL_PNP_FOOTPRINT:
			data.pnp_footprint = value;
			break;
		case COL_OFFSET_X:
			value.ToDouble(&tmp_double);
			data.offset_x = tmp_double;
			break;
		case COL_OFFSET_Y:
			value.ToDouble(&tmp_double);
			data.offset_y = tmp_double;
			break;
		case COL_ANGLE:
			value.ToDouble(&tmp_double);
			data.angle = tmp_double;
			break;
		case COL_ENABLED:
			value.ToLong(&tmp_long);
			data.enabled = tmp_long;
			break;
	}

	//
	// update project
	//
	m_project->UpdateComponents();
	m_project->Notify(wxEVT_PROJECT_UPDATED);

}

void PatternTable::Sort(int col)
{
	switch (col)
	{
		case COL_PATTERN:
			std::sort(m_pattern.begin(), m_pattern.end());
			break;
		case COL_PNP_PACKAGE:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByPnpPackage);
			break;
		case COL_ADD_TO_NAME:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByAddPackToName);
			break;
		case COL_PNP_FOOTPRINT:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByPnpFootprint);
			break;
		case COL_OFFSET_X:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByOffsetX);
			break;
		case COL_OFFSET_Y:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByOffsetY);
			break;
		case COL_ANGLE:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByAngle);
			break;
		case COL_COMP_COUNT:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByCompCount);
			break;
		case COL_ENABLED:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByEnabled);
			break;
		case COL_IS_NEW:
			std::sort(m_pattern.begin(), m_pattern.end(), Pattern::ByIsNew);
			break;
	}
}
