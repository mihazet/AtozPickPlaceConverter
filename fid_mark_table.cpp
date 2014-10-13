#include "fid_mark_table.h"
#include "project.h"

#include <algorithm>

enum
{
	COL_DESIGNATOR = 0,
	COL_NAME,
	COL_LAYER,
	COL_SUBPCB_INDEX,
	COL_USE_ON_SUBPCB,
	COL_USE_GLOBAL,
	COL_LOCATION_X,
	COL_LOCATION_Y,
	COL_LOCAL_FOR,
	COUNT_COLS,
};



FidMarkTable::FidMarkTable(Project *project)
	: wxGridTableBase()
	, m_project(project)
	, m_fidmark(project->GetFidMark())
	, m_component(project->GetComponent())
{
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

FidMarkTable::~FidMarkTable()
{
}

int FidMarkTable::GetNumberRows()
{
	return m_fidmark.size();
}
int FidMarkTable::GetNumberCols()
{
	return COUNT_COLS;
}

wxString FidMarkTable::GetColLabelValue( int col )
{
	wxString result = wxEmptyString;
	switch (col)
	{
		case COL_DESIGNATOR:
			result = "Designator";
			break;
		case COL_NAME:
			result = "Name";
			break;
		case COL_LAYER:
			result = "Layer";
			break;
		case COL_SUBPCB_INDEX:
			result = "SubPCB";
			break;
		case COL_USE_ON_SUBPCB:
			result = "Use as (on subPCB)";
			break;
		case COL_USE_GLOBAL:
			result = "Use as (global)";
			break;
		case COL_LOCATION_X:
			result = "X";
			break;
		case COL_LOCATION_Y:
			result = "Y";
			break;
		case COL_LOCAL_FOR:
			result = "Local for components";
			break;
	}
	return result;
}

wxString FidMarkTable::GetValue(int row, int col)
{

	wxString result = wxEmptyString;
	FidMark& data_fidmark = m_fidmark[row];
	//Component& data_comp = m_component[data_fidmark.component_index];
	int comp_index = data_fidmark.component_index;
	ComponentIt data_comp = std::find_if(m_component.begin(), m_component.end(), CompByIndex(comp_index));

	//	size_t local_for_count;
	switch (col)
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
			result = G_array_on_subpcb[data_fidmark.usage_type];
			break;
		case COL_USE_GLOBAL:
			result = G_array_global[data_fidmark.usage_as_global];
			break;
		case COL_LOCATION_X:
			result = wxString::Format("%f", data_comp->pnp_location_x);
			break;
		case COL_LOCATION_Y:
			result = wxString::Format("%f", data_comp->pnp_location_y);
			break;
		case COL_LOCAL_FOR:
			//	local_for_count = data_fidmark->local_for_comps.GetCount();
			//	for(size_t index = 0; index < local_for_count; index++)
			//	{
			//		result += data_fidmark->local_for_comps[index] + ";";
			//	}
			//	result = result.BeforeLast(';');//FIXME
			break;
	}
	return result;
}

void FidMarkTable::SetValue(int row, int col, const wxString& value)
{

	if ((COL_LOCAL_FOR != col) && value.IsEmpty())
		return;

	FidMark& data_fidmark = m_fidmark[row];
	int comp_index = data_fidmark.component_index;
	ComponentIt data_comp = std::find_if(m_component.begin(), m_component.end(), CompByIndex(comp_index));

	long tmp_long;
	bool single_board = m_project->IsSingleBoard();
	//	wxString component;
	//	bool component_found = false;
	switch (col)
	{
		case COL_USE_ON_SUBPCB:
			tmp_long = G_array_on_subpcb.Index(value, false);
			if (wxNOT_FOUND == tmp_long)
				tmp_long = FID_MARK_USE_UNKNOWN;
			if ((FID_MARK_USE_FM1 == tmp_long) || (FID_MARK_USE_FM2 == tmp_long) || (FID_MARK_USE_FM3 == tmp_long))
			{
				for (int index = m_fidmark.size() - 1; index >= 0; index--)
				{
					FidMark& fm_descr = m_fidmark[index];
					//Component& fm_comp = m_component[fm_descr.component_index];
					int comp_index = fm_descr.component_index;
					ComponentIt fm_comp = std::find_if(m_component.begin(), m_component.end(), CompByIndex(comp_index));

					if ((fm_comp->layer != data_comp->layer) || (fm_comp->pnp_subpcb_index != data_comp->pnp_subpcb_index))
						continue;
					if (tmp_long == fm_descr.usage_type)
					{
						fm_descr.usage_type = data_fidmark.usage_type;
						if (single_board)
							fm_descr.usage_as_global = data_fidmark.usage_as_global;
						break;
					}
				}
			}
			data_fidmark.usage_type = tmp_long;
			if (single_board)
				data_fidmark.usage_as_global = tmp_long;
			break;
		case COL_USE_GLOBAL:
			tmp_long = G_array_global.Index(value, false);
			if (wxNOT_FOUND == tmp_long)
				tmp_long = FID_MARK_USE_UNKNOWN;
			if ((FID_MARK_USE_FM1 == tmp_long) || (FID_MARK_USE_FM2 == tmp_long) || (FID_MARK_USE_FM3 == tmp_long))
			{
				for (int index = m_fidmark.size() - 1; index >= 0; index--)
				{
					FidMark& fm_descr = m_fidmark[index];
					int comp_index = fm_descr.component_index;
					ComponentIt fm_comp = std::find_if(m_component.begin(), m_component.end(), CompByIndex(comp_index));

					if (fm_comp->layer != data_comp->layer)
						continue;
					if (tmp_long == fm_descr.usage_as_global)
					{
						fm_descr.usage_as_global = data_fidmark.usage_as_global;
						if (single_board)
							fm_descr.usage_type = data_fidmark.usage_type;
						break;
					}
				}
			}
			data_fidmark.usage_as_global = (wxNOT_FOUND == tmp_long) ? FID_MARK_USE_UNKNOWN : tmp_long;
			if (single_board)
				data_fidmark.usage_type = tmp_long;
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
	//m_main_data_controller->SaveProjectInfo();
	//m_main_data_controller->Refresh();
	m_project->Notify(wxEVT_PROJECT_UPDATED);
}

void FidMarkTable::Sort(int col)
{
	switch (col)
	{
		case COL_DESIGNATOR:
			break;
		case COL_NAME:
			break;
		case COL_LAYER:
			break;
		case COL_SUBPCB_INDEX:
			break;
		case COL_USE_ON_SUBPCB:
			std::sort(m_fidmark.begin(), m_fidmark.end(), FidMark::ByUsageType);
			break;
		case COL_USE_GLOBAL:
			std::sort(m_fidmark.begin(), m_fidmark.end(), FidMark::ByUsageAsGlobal);
			break;
		case COL_LOCATION_X:
			break;
		case COL_LOCATION_Y:
			break;
		case COL_LOCAL_FOR:
			break;
	}
}
