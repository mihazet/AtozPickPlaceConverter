#ifndef PROJECT_H
#define	PROJECT_H

#include <wx/wx.h>
#include <vector>

#include "common.h"

class wxFileConfig;
class wxTextFile;
class wxXmlNode;
class wxXmlDocument;

/*
  Функторы
 ----------
 Для поиска элемента массива по полю в структуре.
 Использование:
		...
		struct Foo
		{
			wxString name;
		}
		struct FindByName
		{
			FindByName(wxString name) : value(name) {}
			bool operator () (const Foo& foo) { return foo.name == value; }
			wxString value;
		};

		std::vector<Foo> v;
		std::vector<Foo>::iterator found_it = std::find_if(v.begin(), v.end(), FindByName("Hello"));
		...
*/

struct TypeByName
{
	TypeByName(wxString name) : toFind(name) {}
	bool operator () (const ComponentType& type) { return type.name == toFind; }
	wxString toFind;
};

struct PatternByName
{
	PatternByName(wxString name) : toFind(name) {}
	bool operator () (const Pattern& patt) { return patt.pattern == toFind; }
	wxString toFind;
};

struct FidMarkByName
{
	FidMarkByName(wxString name) : toFind(name) {}
	bool operator () (const FidMark& fidmark) { return fidmark.designator == toFind; }
	wxString toFind;
};

struct CompByIndex
{
	CompByIndex(int idx) : toFind(idx) {}
	bool operator () (const Component& comp) { return comp.index == toFind; }
	int toFind;
};

// ----------------------------------------------------------------------------

struct XmlNodePtrs
{
	wxXmlNode *parent;
	wxXmlNode *last_child;
	XmlNodePtrs() : parent(NULL), last_child(NULL) {}
};



struct ItemTemplate
{
	wxString type;
	wxString delimiter;
	wxString format;
	bool skipHeader;

	ItemTemplate(wxString t, wxString d, wxString f, bool skip)
		: type(t), delimiter(d), format(f), skipHeader(skip)
	{
	}
};

class FileTemplate
{
public:
	FileTemplate();
	ItemTemplate *FindByType(wxString type);
private:
	std::vector<ItemTemplate> m_template;
};

class Project : public wxEvtHandler
{
public:
	Project();
	~Project();

	// ---- Load data and parsing, save
	void Load(const wxString& filename, const wxString& type = "pcad");
	void SaveProd(const wxString& filename);
	void SaveDD500File(wxXmlDocument *xmlDoc, wxString file);

	// ---- Propeties
	const wxString&	Filename() { return m_filename; }
	wxString& Name() { return m_project_name; }
	double& Height() { return m_height; }
	int& Angle() { return m_angle; }
	double& SizeX() { return m_size_x; }
	double& SizeY() { return m_size_y; }
	double& OffsetX() { return m_offset_x; }
	double& OffsetY() { return m_offset_y; }
	bool& ApplyOffset()	{ return m_apply_offset; }

	bool IsSingleBoard() const { return m_pcbs.size() <= 1; }

	// ---- Vector Data
	SubpcbVector& Pcbs() { return m_pcbs; }
	ComponentVector& GetComponent() { return m_comps; }
	ComponentTypeVector& GetComponentType() { return m_types; }
	PatternVector& GetPattern() { return m_patterns; }
	FidMarkVector& GetFidMark() { return m_fidmarks; }

	// ---- Updates
	void UpdateComponents();
	void UpdatePCBFullSize();

	// ---- Update GUI
	void Notify(wxEventType type);

private:

	// Обработка ланных
	bool Parse(const wxString& filename, ItemTemplate *templ);
	void UpdateComponent(Component& comp, size_t compIndex);
	void UpdateComponentFullName();
	void LoadTypes();
	void SaveTypes();
	void LoadPatterns();
	void SavePatterns();
	// Экспорт
	void PrintDD500SideDescr(wxTextFile& doc, wxString side);
	wxXmlNode *CreateProductSideDescr(wxString side);
	void PrintFiducial(XmlNodePtrs *pNode, Component comp);
	void PrintComponent(XmlNodePtrs *pNode, Component comp);
	void LoadProjectInfo(wxString filename);
	void SaveProjectInfo();
	// работа с олним элементом
	void LoadComponent(ComponentType& compType, const Component& comp);
	void LoadPattern(Pattern& pattern);
	// Находится ли плата в заданных габаритах
	bool IsInRoom(double ref1_x, double ref1_y, double ref2_x, double ref2_y, double ptr_x, double ptr_y);

	// Сами данные, которые обрабатываем
	ComponentVector m_comps;
	ComponentTypeVector m_types;
	PatternVector m_patterns;
	FidMarkVector m_fidmarks;
	SubpcbVector m_pcbs;
	FileTemplate m_fileTempl;

	// параметры проекта
	wxString 	m_project_name;	/**< имя проекта (пока не используется) */
	double		m_height;		/**< толщина платы */
	int			m_angle;  		/**< Угол, под которым плата загружентся в машину (0=0, 1=90, 2=180, 3=270) */
	double		m_size_x;  		/**< полный размер платы */
	double		m_size_y;  		/**< полный размер платы */
	double		m_offset_x;		/**< глобальное смещение платы */
	double		m_offset_y;		/**< глобальное смещение платы */
	bool		m_apply_offset;	/**< координаты компонентов уже сдвинуты на offset - надо передвинуть обратно */
	wxString 	m_filename;

	// настройка
	wxFileConfig *m_settingsCfg;
	wxFileConfig *m_compPcadCfg;
	wxFileConfig *m_compAltumCfg;
	wxFileConfig *m_patternsPcadCfg;
	wxFileConfig *m_patternsAltumCfg;
	wxFileConfig *m_projectsCfg;
	// Активный конфиг
	wxFileConfig *m_activeCompsCfg;
	wxFileConfig *m_activePatternsCfg;
};

wxDECLARE_EVENT(wxEVT_PROJECT_LOADED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PROJECT_UPDATED, wxCommandEvent);

#define	EVT_PROJECT_LOADED(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_LOADED, func)

#define	EVT_PROJECT_UPDATED(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_UPDATED, func)

#endif // PROJECT_H

