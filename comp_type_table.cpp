#include "comp_type_table.h"
#include "project.h"

#include <algorithm>

enum
{
	COL_NAME = 0,
	COL_PATTERN,
	COL_PNP_NAME,
	COL_OVR_NAME,
	COL_COMP_COUNT,
	COL_ENABLED,
	COL_IS_NEW,
	COUNT_COLS,
};


ComponentTypeTable::ComponentTypeTable(Project *project)
	: wxGridTableBase()
	, m_project(project)
	, m_type(project->GetComponentType())
{

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
	SetColAttr (bool_attr, COL_OVR_NAME);

	//	InsertColumn(COL_NAME,		_T("Name"),	wxLIST_FORMAT_LEFT, 120);
	//	InsertColumn(COL_PATTERN,	_T("Patt"),	wxLIST_FORMAT_LEFT, 70);
	//	InsertColumn(COL_PNP_NAME,	_T("PNP Name"),	wxLIST_FORMAT_LEFT, 110);
	//	InsertColumn(COL_COMP_COUNT,	_T("Count"),	wxLIST_FORMAT_LEFT, 50);
	//	InsertColumn(COL_ENABLED,	_T("To OUT"),	wxLIST_FORMAT_LEFT, 50);
	//	InsertColumn(COL_IS_NEW,	_T("New"),	wxLIST_FORMAT_LEFT, 50);
}

ComponentTypeTable::~ComponentTypeTable()
{
}

int ComponentTypeTable::GetNumberRows()
{
	return m_type.size();
}
int ComponentTypeTable::GetNumberCols()
{
	return COUNT_COLS;
}

wxString ComponentTypeTable::GetColLabelValue( int col )
{
	wxString result = wxEmptyString;
	switch (col)
	{
		case COL_NAME:
			result = "Name";
			break;
		case COL_PATTERN:
			result = "Pattern";
			break;
		case COL_PNP_NAME:
			result = "Name in PnP";
			break;
		case COL_OVR_NAME:
			result = "Override Name";
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

wxString ComponentTypeTable::GetValue(int row, int col)
{
	wxString result = wxEmptyString;
	ComponentType& data = m_type[row];
	switch (col)
	{
		case COL_NAME:
			result = data.name;
			break;
		case COL_PATTERN:
			result = data.pattern;
			break;
		case COL_PNP_NAME:
			result = data.pnp_name;
			break;
		case COL_OVR_NAME:
			result = wxString::Format("%d", data.override_name);
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

void ComponentTypeTable::SetValue(int row, int col, const wxString& value)
{
	long tmp_long;

	if (value.IsEmpty())
		return;

	ComponentType& data = m_type[row];
	switch (col)
	{
		case COL_PATTERN:
			data.pattern = value;
			break;
		case COL_PNP_NAME:
			data.pnp_name = value;
			break;
		case COL_OVR_NAME:
			value.ToLong(&tmp_long);
			data.override_name = tmp_long;
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

void ComponentTypeTable::Sort(int col)
{
	switch (col)
	{
		case COL_NAME:
			// по умолчанию - оператор '<'
			std::sort(m_type.begin(), m_type.end());
			break;
		case COL_PATTERN:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByPattern);
			break;
		case COL_PNP_NAME:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByPnpName);
			break;
		case COL_OVR_NAME:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByOverrideName);
			break;
		case COL_COMP_COUNT:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByCompCount);
			break;
		case COL_ENABLED:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByEnabled);
			break;
		case COL_IS_NEW:
			std::sort(m_type.begin(), m_type.end(), ComponentType::ByIsNew);
			break;
	}
}
