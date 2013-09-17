#ifndef COMMON_H
#define COMMON_H

#include <wx/dynarray.h>
#include <wx/string.h>

class wxXmlNode;

struct t_component_descr {
///исходные данные
	wxString	designator;/**< Обозначение компонента */
	wxString	cad_name;/**< Имя компонента в PCAD/Altium */
	wxString	cad_pattern;/**< Имя посадочного места в PCAD/Altium */
	wxString	cad_value;/**< Номинал компонента, или его название */
	wxString	full_name;/**< Идентификатор, расчитанный на базе имён и номинала */
	wxString	layer;/**< Слой, в котором находится компонент */
	double		cad_location_x;/**< координата реферной точки компонента в PCAD/Altium */
	double		cad_location_y;/**< координата реферной точки компонента в PCAD/Altium */
	double		cad_angle;/**< угол поворота компонента в PCAD/Altium */
///рассчитанные значения
	wxString	strip_value;/**< Номинал компонента, или его название после удаления маркера DNP*/
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_name;/**< Имя компонента в PP-050/DD-500 */
	wxString	pnp_package;/**< Имя корпуса в PP-050 */
	wxString	pnp_footprint;/**< Имя футпринта в DD-500 */
	double		pnp_location_x;/**< координата центра компонента в PP-050/DD-500 */
	double		pnp_location_y;/**< координата центра компонента в PP-050/DD-500 */
	double		pnp_angle;/**< угол поворота компонента в PP-050/DD-500 */
	bool		enabled;/**< Устанавливается ли компонент в PP-050/DD-500 */
	t_component_descr() :
		cad_location_x(0), cad_location_y(0), cad_angle(0),
		pnp_location_x(0), pnp_location_y(0), pnp_angle(0),
		enabled(true) {}
};

struct t_component_type_descr {
	wxString	name;/**< Идентификатор, расчитанный на базе имён и номинала в PCAD/Altium */
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_name;/**< Имя компонента в PP-050/DD-500 */
	double		value;/**< Номинальное значение для резисторов/еондёров/etc. */
	wxString	unit;/**< Размерность номинального значения */
	wxString	value_postfix;/**< Утточнение номинала (например вольтаж кондёра) */
	size_t		comp_count;/**< Количество компонентов этого типа на плате */
	bool		enabled;/**< Устанавливается ли компонент в PP-050/DD-500 */
	bool		is_new;/**< Был ли этот компонент в базе конвертора */
	t_component_type_descr() : value(0), comp_count(1), enabled(true), is_new(true) {}
};

struct t_pattern_descr {
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_package;/**< Имя корпуса в PP-050 */
	wxString	pnp_footprint;/**< Имя футпринта в DD-500 */
	double		offset_x;/**< смещение центра компонента от реферной точки */
	double		offset_y;/**< смещение центра компонента от реферной точки */
	double		angle;/**< угол поворота компонента в базе PCAD/Altium относительно PP-050/DD-500 */
	size_t		comp_count;/**< Количество компонентов с таким корпусом на плате */
	bool		enabled;/**< Устанавливается ли компонент с таким корпусом в PP-050/DD-500 */
	bool		is_new;/**< Был ли этот корпус в базе конвертора */
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

struct t_subpcb_descr {
	wxString	subpcb_name; /**< Имя платы */
	double		ref_point1_x;/**< Координаты угла платы */
	double		ref_point1_y;/**< Координаты угла платы */
	double		ref_point2_x;/**< Координаты угла платы */
	double		ref_point2_y;/**< Координаты угла платы */
	double		size_x;  /**< размер платы */
	double		size_y;  /**< размер платы */
	double		offset_x;/**< смещение платы */
	double		offset_y;/**< смещение платы */
	t_subpcb_descr() : ref_point1_x(0), ref_point1_y(0), ref_point2_x(0), ref_point2_y(0),
		size_x(0), size_y(0), offset_x(0), offset_y(0) {}
};
WX_DECLARE_OBJARRAY(struct t_subpcb_descr, tSubPcbs);
struct t_board_descr {
	wxString	project_name;/**< Имя проекта (пока не используем) */
	wxString	filename;    /**< Имя файла, экспортированноего из CAD */
	wxString	fullfilename;/**< Имя файла, экспортированноего из CAD */
	double		height;  /**< толщина платы */
	tSubPcbs	pcbs; /**< Габариты плат на листе */
	t_board_descr() : height(1.6) {}
};

int CmpCompTypeFunc(t_component_type_descr *a_arg1, t_component_type_descr *a_arg2);
int CmpPatternFunc(t_pattern_descr *a_arg1, t_pattern_descr *a_arg2);

#endif // COMMON_H
