#ifndef COMMON_H
#define COMMON_H

#include <wx/dynarray.h>

class wxXmlNode;
class wxString;

struct t_component_descr {
	wxString designator;
	wxString pattern;
	wxString name;
	wxString value;
	wxString layer;
	double location_x;
	double location_y;
	double angle;
	bool enabled;
};

struct t_xml_node_ptrs {
	wxXmlNode *parent;
	wxXmlNode *last_child;
	int elemets_count;
};

WX_DECLARE_OBJARRAY(struct t_component_descr, tComponentDescr);

#endif // COMMON_H
