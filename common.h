#ifndef COMMON_H
#define COMMON_H

#include <wx/wx.h>
#include <vector>

#define LAYER_TOP_NAME		("T")
#define LAYER_BOT_NAME		("B")
#define FIDMARD_DES_PREF	("FM")

static bool CompareString(wxString s1, wxString s2)
{
	if (s1.length() == s2.length())
		return s1 < s2;
	else
		return s1.length() < s2.length();
}


// common types
// from Alatar's proect
//

struct Component
{
	// �������� ������
	wxString	designator;		// ����������� ����������
	wxString	cad_name;		// ��� ���������� � PCAD/Altium
	wxString	cad_pattern;	// ��� ����������� ����� � PCAD/Altium
	wxString	cad_value;		// ������� ����������, ��� ��� ��������
	wxString	full_name;		// �������������, ����������� �� ���� ��� � ��������
	wxString	layer;			// ����, � ������� ��������� ���������
	double		cad_location_x;	// ���������� �������� ����� ���������� � PCAD/Altium
	double		cad_location_y;	// ���������� �������� ����� ���������� � PCAD/Altium
	double		cad_angle;		// ���� �������� ���������� � PCAD/Altium

	// ������������ ��������
	bool		enabled;		// ��������������� �� ��������� � PP-050/DD-500
	wxString	strip_value;	// ������� ����������, ��� ��� �������� ����� �������� ������� DNP
	wxString	pattern;		// ��� ����������� ����� � ���� ����������
	wxString	pnp_name;		// ��� ���������� � PP-050/DD-500
	wxString	pnp_package;	// ��� ������� � PP-050
	wxString	pnp_footprint;	// ��� ��������� � DD-500
	double		pnp_location_x;	// ���������� ������ ���������� � PP-050/DD-500
	double		pnp_location_y;	// ���������� ������ ���������� � PP-050/DD-500
	double		pnp_angle;		// ���� �������� ���������� � PP-050/DD-500
	bool		pnp_enabled;	// ��������������� �� ��������� � PP-050/DD-500
	int			pnp_subpcb_index;	// � ������ ����� ����� ���������

	int 		index;			// ������ ����������, �� ���� �������� FidMark

	Component()
		: cad_location_x(0), cad_location_y(0), cad_angle(0), enabled(true),
		  pnp_location_x(0), pnp_location_y(0), pnp_angle(0), pnp_enabled(true),
		  pnp_subpcb_index(0), index(-1)
	{
	}

	// ���������� �� ��������� �� ����������� ���������� (designator)
	bool operator < (const Component& comp) const { return CompareString(designator, comp.designator); }
	// ����� ���������� �� ��������� �����
	static bool ByCadName(const Component& arg1, const Component& arg2) { return arg1.cad_name < arg2.cad_name; }
	static bool ByCadPattern(const Component& arg1, const Component& arg2) { return arg1.cad_pattern < arg2.cad_pattern; }
	static bool ByCadValue(const Component& arg1, const Component& arg2) { return arg1.cad_value < arg2.cad_value; }
	static bool ByFullName(const Component& arg1, const Component& arg2) { return arg1.cad_value < arg2.cad_value; }
	static bool ByLayer(const Component& arg1, const Component& arg2) { return arg1.cad_value < arg2.cad_value; }
	static bool ByCadLocationX(const Component& arg1, const Component& arg2) { return arg1.cad_location_x < arg2.cad_location_x; }
	static bool ByCadLocationY(const Component& arg1, const Component& arg2) { return arg1.cad_location_y < arg2.cad_location_y; }
	static bool ByCadAngle(const Component& arg1, const Component& arg2) { return arg1.cad_angle < arg2.cad_angle; }
	static bool ByEnabled(const Component& arg1, const Component& arg2) { return arg1.enabled < arg2.enabled; }
	static bool ByStripValue(const Component& arg1, const Component& arg2) { return arg1.strip_value < arg2.strip_value; }
	static bool ByPattern(const Component& arg1, const Component& arg2) { return arg1.pattern < arg2.pattern; }
	static bool ByPnpName(const Component& arg1, const Component& arg2) { return arg1.pnp_name < arg2.pnp_name; }
	static bool ByPnpPackage(const Component& arg1, const Component& arg2) { return arg1.pnp_package < arg2.pnp_package; }
	static bool ByPnpFootprint(const Component& arg1, const Component& arg2) { return arg1.pnp_footprint < arg2.pnp_footprint; }
	static bool ByPnpLocationX(const Component& arg1, const Component& arg2) { return arg1.pnp_location_x < arg2.pnp_location_x; }
	static bool ByPnpLocationY(const Component& arg1, const Component& arg2) { return arg1.pnp_location_y < arg2.pnp_location_y; }
	static bool ByPnpAngle(const Component& arg1, const Component& arg2) { return arg1.pnp_angle < arg2.pnp_angle; }
	static bool ByPnpEnabled(const Component& arg1, const Component& arg2) { return arg1.pnp_enabled < arg2.pnp_enabled; }
	static bool ByPnpSubpcbIndex(const Component& arg1, const Component& arg2) { return arg1.pnp_subpcb_index < arg2.pnp_subpcb_index; }

};

struct ComponentType
{
	wxString	name;			// �������������, ����������� �� ���� ��� � �������� � PCAD/Altium
	wxString	pattern;		// ��� ����������� ����� � ���� ����������
	wxString	pnp_name;		// ��� ���������� � PP-050/DD-500
	bool		override_name;	// �� �������� ��� �� ������� � ��������
	wxString	value;			// ����������� �������� ��� ����������/�������/etc. ��� ������ �������� ����������
	int			comp_count;		// ���������� ����������� ����� ���� �� �����
	bool		enabled;		// ��������������� �� ��������� � PP-050/DD-500
	bool		is_new;			// ��� �� ���� ��������� � ���� ����������

	ComponentType()
		: override_name(false), comp_count(1), enabled(true), is_new(true)
	{
	}

	// ���������� �� ��������� �� name
	bool operator < (const ComponentType& type) const { return name < type.name; }
	// ����� ���������� �� ��������� �����
	static bool ByPattern(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.pattern < arg2.pattern; }
	static bool ByPnpName(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.pnp_name < arg2.pnp_name; }
	static bool ByOverrideName(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.override_name < arg2.override_name; }
	static bool ByValue(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.value < arg2.value; }
	static bool ByCompCount(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.comp_count < arg2.comp_count; }
	static bool ByEnabled(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.enabled < arg2.enabled; }
	static bool ByIsNew(const ComponentType& arg1, const ComponentType& arg2) {	return arg1.is_new < arg2.is_new; }
};

struct Pattern
{
	wxString	pattern;			// ��� ����������� ����� � ���� ����������
	wxString	pnp_package;		// ��� ������� � PP-050
	bool		add_pack_to_name;	// ��������� ��� ������� � ����� ����������
	wxString	pnp_footprint;		// ��� ��������� � DD-500
	double		offset_x;			// �������� ������ ���������� �� �������� �����
	double		offset_y;			// �������� ������ ���������� �� �������� �����
	double		angle;				// ���� �������� ���������� � ���� PCAD/Altium ������������ PP-050/DD-500
	int			comp_count;			// ���������� ����������� � ����� �������� �� �����
	bool		enabled;			// ��������������� �� ��������� � ����� �������� � PP-050/DD-500
	bool		is_new;				// ��� �� ���� ������ � ���� ����������
	Pattern()
		: add_pack_to_name(false),
		  offset_x(0), offset_y(0), angle(0),
		  comp_count(1), enabled(true), is_new(true)
	{
	}

	// ���������� �� ��������� �� pattern
	bool operator < (const Pattern& type) const { return pattern < type.pattern; }
	// ����� ���������� �� ��������� �����
	static bool ByPattern(const Pattern& arg1, const Pattern& arg2) {	return arg1.pattern < arg2.pattern; }
	static bool ByPnpPackage(const Pattern& arg1, const Pattern& arg2) {	return arg1.pnp_package < arg2.pnp_package; }
	static bool ByAddPackToName(const Pattern& arg1, const Pattern& arg2) {	return arg1.add_pack_to_name < arg2.add_pack_to_name; }
	static bool ByPnpFootprint(const Pattern& arg1, const Pattern& arg2) {	return arg1.pnp_footprint < arg2.pnp_footprint; }
	static bool ByOffsetX(const Pattern& arg1, const Pattern& arg2) {	return arg1.offset_x < arg2.offset_x; }
	static bool ByOffsetY(const Pattern& arg1, const Pattern& arg2) {	return arg1.offset_y < arg2.offset_y; }
	static bool ByAngle(const Pattern& arg1, const Pattern& arg2) {	return arg1.angle < arg2.angle; }
	static bool ByCompCount(const Pattern& arg1, const Pattern& arg2) {	return arg1.comp_count < arg2.comp_count; }
	static bool ByEnabled(const Pattern& arg1, const Pattern& arg2) {	return arg1.enabled < arg2.enabled; }
	static bool ByIsNew(const Pattern& arg1, const Pattern& arg2) {	return arg1.is_new < arg2.is_new; }

};


enum
{
	FID_MARK_USE_UNKNOWN = 0,
	FID_MARK_USE_IGNORE,
	FID_MARK_USE_FM1,
	FID_MARK_USE_FM2,
	FID_MARK_USE_FM3,
	FID_MARK_USE_LOCAL
};

struct FidMark
{
	wxString	designator;
	int			component_index;	// == ComponentDescr::index
	int			usage_type;
	int			usage_as_global;
	//	wxArrayString	local_for_comps;
	FidMark()
		: component_index(0), usage_type(FID_MARK_USE_UNKNOWN), usage_as_global(FID_MARK_USE_UNKNOWN)
	{
	}

	// ���������� �� ��������� �� ����������� ���������� (designator)
	bool operator < (const FidMark& type) const { return designator < type.designator; }
	// ����� ���������� �� ��������� �����
	static bool ByComponentIndex(const FidMark& arg1, const FidMark& arg2) {	return arg1.component_index < arg2.component_index; }
	static bool ByUsageType(const FidMark& arg1, const FidMark& arg2) {	return arg1.usage_type < arg2.usage_type; }
	static bool ByUsageAsGlobal(const FidMark& arg1, const FidMark& arg2) {	return arg1.usage_as_global < arg2.usage_as_global; }

};

struct Subpcb
{
	wxString	subpcb_name; 	// ��� �����
	double		ref_point1_x;	// ���������� ���� �����
	double		ref_point1_y;	// ���������� ���� �����
	double		ref_point2_x;	// ���������� ���� �����
	double		ref_point2_y;	// ���������� ���� �����
	double		size_x;  		// ������ �����
	double		size_y;  		// ������ �����
	double		offset_x;		// �������� �����
	double		offset_y;		// �������� �����
	bool		enabled; 		// �� �������� ��� �����
	Subpcb()
		: ref_point1_x(0), ref_point1_y(0), ref_point2_x(0), ref_point2_y(0),
		  size_x(0), size_y(0), offset_x(0), offset_y(0),
		  enabled(true)
	{
	}
};


typedef std::vector<Component> ComponentVector;
typedef std::vector<ComponentType> ComponentTypeVector;
typedef std::vector<Pattern> PatternVector;
typedef std::vector<FidMark> FidMarkVector;
typedef std::vector<Subpcb> SubpcbVector;

typedef std::vector<Component>::iterator ComponentIt;
typedef std::vector<ComponentType>::iterator ComponentTypeIt;
typedef std::vector<Pattern>::iterator PatternIt;
typedef std::vector<FidMark>::iterator FidMarkIt;
typedef std::vector<Subpcb>::iterator SubpcbIt;


extern wxArrayString G_array_on_subpcb;
extern wxArrayString G_array_global;


#endif // COMMON_H

