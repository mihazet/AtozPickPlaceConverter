#ifndef COMMON_H
#define COMMON_H

#include <wx/dynarray.h>
#include <wx/string.h>

class wxXmlNode;

struct t_component_descr {
	wxString designator;
	wxString pattern;
	wxString src_name;
	wxString name;
	wxString value;
	wxString layer;
	double location_x;
	double location_y;
	double angle;
	bool enabled;
        t_component_descr() : location_x(0), location_y(0), angle(0), enabled(true) {}
};

struct t_component_type_descr {
	wxString name;
	wxString pattern;
	wxString rename_to;
	size_t comp_count;
	bool enabled;
	bool is_new;
        t_component_type_descr() : comp_count(1), enabled(true), is_new(true) {}
};

struct t_pattern_descr {
	wxString pattern;
	double offset_x;
	double offset_y;
	double angle;
	size_t comp_count;
	bool enabled;
	bool is_new;
        t_pattern_descr() : offset_x(0), offset_y(0), angle(0),
        	comp_count(1), enabled(true), is_new(true) {}
};

struct t_xml_node_ptrs {
	wxXmlNode *parent;
	wxXmlNode *last_child;
	int elemets_count;
};

WX_DECLARE_OBJARRAY(struct t_component_descr, tComponentDescr);
WX_DEFINE_SORTED_ARRAY(struct t_component_type_descr *, tComponentTypeDescr);
WX_DEFINE_SORTED_ARRAY(struct t_pattern_descr *, tPatternDescr);

int CmpCompTypeFunc(t_component_type_descr *a_arg1, t_component_type_descr *a_arg2);
int CmpPatternFunc(t_pattern_descr *a_arg1, t_pattern_descr *a_arg2);

#endif // COMMON_H
