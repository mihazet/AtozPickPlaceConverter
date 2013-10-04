#ifndef COMMON_H
#define COMMON_H

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/arrstr.h>

class wxXmlNode;

#define LAYER_TOP_NAME		("T")
#define LAYER_BOT_NAME		("B")
#define FIDMARD_DES_PREF	("FM")

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
	bool		enabled;/**< Устанавливается ли компонент в PP-050/DD-500 */
	wxString	strip_value;/**< Номинал компонента, или его название после удаления маркера DNP*/
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_name;/**< Имя компонента в PP-050/DD-500 */
	wxString	pnp_package;/**< Имя корпуса в PP-050 */
	wxString	pnp_footprint;/**< Имя футпринта в DD-500 */
	double		pnp_location_x;/**< координата центра компонента в PP-050/DD-500 */
	double		pnp_location_y;/**< координата центра компонента в PP-050/DD-500 */
	double		pnp_angle;/**< угол поворота компонента в PP-050/DD-500 */
	bool		pnp_enabled;/**< Устанавливается ли компонент в PP-050/DD-500 */
	int		pnp_subpcb_index;/**< К какому куску платы относится */
	t_component_descr() :
		cad_location_x(0), cad_location_y(0), cad_angle(0), enabled(true),
		pnp_location_x(0), pnp_location_y(0), pnp_angle(0), pnp_enabled(true),
		pnp_subpcb_index(0) {}
};

struct t_component_type_descr {
	wxString	name;/**< Идентификатор, расчитанный на базе имён и номинала в PCAD/Altium */
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_name;/**< Имя компонента в PP-050/DD-500 */
	bool		override_name;/**< Не генерить имя из шаблона и названия */
	wxString	value;/**< Номинальное значение для резисторов/еондёров/etc. или просто название компонента*/
	int		comp_count;/**< Количество компонентов этого типа на плате */
	bool		enabled;/**< Устанавливается ли компонент в PP-050/DD-500 */
	bool		is_new;/**< Был ли этот компонент в базе конвертора */
	t_component_type_descr() : override_name(false), comp_count(1), enabled(true), is_new(true) {}
};

struct t_pattern_descr {
	wxString	pattern;/**< Имя посадочного места в базе конвертора */
	wxString	pnp_package;/**< Имя корпуса в PP-050 */
	bool		add_pack_to_name;/**< Добавлять имя корпуса к имени компонента */
	wxString	pnp_footprint;/**< Имя футпринта в DD-500 */
	double		offset_x;/**< смещение центра компонента от реферной точки */
	double		offset_y;/**< смещение центра компонента от реферной точки */
	double		angle;/**< угол поворота компонента в базе PCAD/Altium относительно PP-050/DD-500 */
	int		comp_count;/**< Количество компонентов с таким корпусом на плате */
	bool		enabled;/**< Устанавливается ли компонент с таким корпусом в PP-050/DD-500 */
	bool		is_new;/**< Был ли этот корпус в базе конвертора */
	t_pattern_descr() : add_pack_to_name(false),
		offset_x(0), offset_y(0), angle(0),
		comp_count(1), enabled(true), is_new(true) {}
};

enum eFidMarkUse {
	FID_MARK_USE_UNKNOWN = 0,
	FID_MARK_USE_IGNORE,
	FID_MARK_USE_FM1,
	FID_MARK_USE_FM2,
	FID_MARK_USE_FM3,
	FID_MARK_USE_LOCAL
};

struct t_fid_mark_descr {
	wxString	designator;/**< Десигнатор реперной точки ()дублирование информации, но нужно */
	int		component_index;/**< Индекс реперной точки в списке компонентов */
	int		usage_type;/**< Как использовать */
	int		usage_as_global;/**< Глобальный репер (для всей панели) */
//	wxArrayString	local_for_comps;/**< Список компонентов, с которыми ассоциирован этот локальный репер (через ";") */
	t_fid_mark_descr() :
		component_index(0), usage_type(FID_MARK_USE_UNKNOWN), usage_as_global(FID_MARK_USE_UNKNOWN) {}
};

struct t_xml_node_ptrs {
	wxXmlNode *parent;
	wxXmlNode *last_child;
	t_xml_node_ptrs() : parent(NULL), last_child(NULL) {}
};

WX_DECLARE_OBJARRAY(struct t_component_descr, tComponentDescr);
WX_DEFINE_SORTED_ARRAY(struct t_component_type_descr *, tComponentTypeDescr);
WX_DEFINE_SORTED_ARRAY(struct t_pattern_descr *, tPatternDescr);
WX_DEFINE_SORTED_ARRAY(struct t_fid_mark_descr *, tFidMarkDescr);

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
	bool		enabled; /**< не собирать эту плату */
	t_subpcb_descr() : ref_point1_x(0), ref_point1_y(0), ref_point2_x(0), ref_point2_y(0),
		size_x(0), size_y(0), offset_x(0), offset_y(0),
		enabled(true) {}
};
WX_DECLARE_OBJARRAY(struct t_subpcb_descr, tSubPcbs);
struct t_board_descr {
	wxString	project_name;/**< Имя проекта (пока не используем) */
	wxString	filename;    /**< Имя файла, экспортированноего из CAD */
	wxString	fullfilename;/**< Имя файла, экспортированноего из CAD */
	double		height;  /**< толщина платы */
	int		angle;  /**< Угол, под которым плата загружентся в машину (0=0, 1=90, 2=180, 3=270) */
	double		size_x;  /**< полный размер платы */
	double		size_y;  /**< полный размер платы */
	double		offset_x;/**< глобальное смещение платы */
	double		offset_y;/**< глобальное смещение платы */
	bool		apply_offset;/**< координаты компонентов уже сдвинуты на offset - надо передвинуть обратно */
	tSubPcbs	pcbs; /**< Габариты плат на листе */
	t_board_descr() : height(1.6), angle(0),
		size_x(0), size_y(0), offset_x(0), offset_y(0),
		apply_offset(false) {}
};

int CmpCompTypeFunc(t_component_type_descr *a_arg1, t_component_type_descr *a_arg2);
int CmpPatternFunc(t_pattern_descr *a_arg1, t_pattern_descr *a_arg2);
int CmpFidMarkFunc(t_fid_mark_descr *a_arg1, t_fid_mark_descr *a_arg2);

extern wxArrayString G_array_on_subpcb;
extern wxArrayString G_array_global;


#endif // COMMON_H
