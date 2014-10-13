#include "comp_table.h"
#include "project.h"

#include <algorithm>

enum ComponentColumns
{
	COL_DESIGNATOR = 0,
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
	COUNT,
};


ComponentTable::ComponentTable(Project *project)
	: wxGridTableBase()
	, m_project(project)
	, m_component(project->GetComponent())
{
}

ComponentTable::~ComponentTable()
{
}

int ComponentTable::GetNumberRows()
{
	return m_component.size();
}

int ComponentTable::GetNumberCols()
{
	return COUNT;
}

wxString ComponentTable::GetColLabelValue( int col )
{
	wxString result = wxEmptyString;
	switch (col)
	{
		case COL_DESIGNATOR:
			result = "Des";
			break;
		case COL_CAD_NAME:
			result = "Name";
			break;
		case COL_CAD_PATTERN:
			result = "Pattern";
			break;
		case COL_CAD_VALUE:
			result = "Value";
			break;
		case COL_FULL_NAME:
			result = "ID";
			break;
		case COL_LAYER:
			result = "Layer";
			break;
		case COL_CAD_LOCATION_X:
			result = "X";
			break;
		case COL_CAD_LOCATION_Y:
			result = "Y";
			break;
		case COL_CAD_ANGLE:
			result = "Angle";
			break;
		case COL_ENABLED:
			result = "To OUT";
			break;
		case COL_PATTERN:
			result = "Patt";
			break;
		case COL_PNP_NAME:
			result = "PNP Comp";
			break;
		case COL_PNP_PACKAGE:
			result = "PNP Pack";
			break;
		case COL_PNP_FOOTPRINT:
			result = "PNP Foot";
			break;
		case COL_PNP_LOCATION_X:
			result = "PNP X";
			break;
		case COL_PNP_LOCATION_Y:
			result = "PNP Y";
			break;
		case COL_PNP_ANGLE:
			result = "PNP Angle";
			break;
		case COL_PNP_SUBPCB:
			result = "PNP pcb";
			break;
		case COL_PNP_ENABLED:
			result = "To OUT";
			break;

	}
	return result;
}

wxString ComponentTable::GetValue( int row, int col )
{
	wxString result = wxEmptyString;
	Component& data = m_component[row];
	switch (col)
	{
		case COL_DESIGNATOR:
			result = data.designator;
			break;
		case COL_CAD_NAME:
			result = data.cad_name;
			break;
		case COL_CAD_PATTERN:
			result = data.cad_pattern;
			break;
		case COL_CAD_VALUE:
			result = data.cad_value;
			break;
		case COL_FULL_NAME:
			result = data.full_name;
			break;
		case COL_LAYER:
			result = data.layer;
			break;
		case COL_CAD_LOCATION_X:
			result = wxString::Format("%.3f", data.cad_location_x);
			break;
		case COL_CAD_LOCATION_Y:
			result = wxString::Format("%.3f", data.cad_location_y);
			break;
		case COL_CAD_ANGLE:
			result = wxString::Format("%.1f", data.cad_angle);
			break;
		case COL_ENABLED:
			result = wxString::Format("%d", data.enabled);
			break;
		case COL_PATTERN:
			result = data.pattern;
			break;
		case COL_PNP_NAME:
			result = data.pnp_name;
			break;
		case COL_PNP_PACKAGE:
			result = data.pnp_package;
			break;
		case COL_PNP_FOOTPRINT:
			result = data.pnp_footprint;
			break;
		case COL_PNP_LOCATION_X:
			result = wxString::Format("%.3f", data.pnp_location_x);
			break;
		case COL_PNP_LOCATION_Y:
			result = wxString::Format("%.3f", data.pnp_location_y);
			break;
		case COL_PNP_ANGLE:
			result = wxString::Format("%.1f", data.pnp_angle);
			break;
		case COL_PNP_SUBPCB:
			result = wxString::Format("%d", data.pnp_subpcb_index);
			break;
		case COL_PNP_ENABLED:
			result = wxString::Format("%d", data.pnp_enabled);
			break;
	}
	return result;
}

void ComponentTable::SetValue( int row, int col, const wxString& value )
{
}

void ComponentTable::Sort(int col)
{
	switch (col)
	{
		case COL_DESIGNATOR:
			std::sort(m_component.begin(), m_component.end());
			break;
		case COL_CAD_NAME:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadName);
			break;
		case COL_CAD_PATTERN:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadPattern);
			break;
		case COL_CAD_VALUE:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadValue);
			break;
		case COL_FULL_NAME:
			std::sort(m_component.begin(), m_component.end(), Component::ByFullName);
			break;
		case COL_LAYER:
			std::sort(m_component.begin(), m_component.end(), Component::ByLayer);
			break;
		case COL_CAD_LOCATION_X:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadLocationX);
			break;
		case COL_CAD_LOCATION_Y:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadLocationY);
			break;
		case COL_CAD_ANGLE:
			std::sort(m_component.begin(), m_component.end(), Component::ByCadAngle);
			break;
		case COL_ENABLED:
			std::sort(m_component.begin(), m_component.end(), Component::ByEnabled);
			break;
		case COL_PATTERN:
			std::sort(m_component.begin(), m_component.end(), Component::ByPattern);
			break;
		case COL_PNP_NAME:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpName);
			break;
		case COL_PNP_PACKAGE:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpPackage);
			break;
		case COL_PNP_FOOTPRINT:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpFootprint);
			break;
		case COL_PNP_LOCATION_X:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpLocationX);
			break;
		case COL_PNP_LOCATION_Y:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpLocationY);
			break;
		case COL_PNP_ANGLE:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpAngle);
			break;
		case COL_PNP_SUBPCB:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpSubpcbIndex);
			break;
		case COL_PNP_ENABLED:
			std::sort(m_component.begin(), m_component.end(), Component::ByPnpEnabled);
			break;
	}

}
