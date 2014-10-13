#include <wx/wx.h>
#include <vector>

#include "common.h"

class wxFileConfig;
class wxTextFile;
class wxXmlNode;
class wxXmlDocument;

enum
{
	INDEX_TOP_COMP = 0,
	INDEX_TOP_FID,
	INDEX_BOT_COMP,
	INDEX_BOT_FID,
	COUNT_INDEX,
};

/*
  ��������
 ----------
 ��� ������ �������� ������� �� ���� � ���������.
 �������������:
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

	// ---- Load/Save project
	void OpenProject(const wxString& filename);
	void SaveProject(const wxString& filename = wxEmptyString);

	void AddCadFile(const wxString& filename);
	wxString FindCadFileByName(const wxString& name);

	// ---- Load data and parsing, save
	void Load(const wxString& filename);
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
	wxArrayString CadFiles() { return m_cadfiles; }

	bool IsNewProject() const { return m_newProject; }
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
	void Notify(wxEventType type, const wxString& msg = wxEmptyString);

private:

	// ��������� ������
	//
	bool Parse(const wxString& filename, ItemTemplate *templ);
	void UpdateComponent(Component& comp, size_t compIndex);
	void UpdateComponentFullName();
	void LoadTypes();
	void SaveTypes();
	void LoadPatterns();
	void SavePatterns();

	// �������
	//
	void PrintDD500SideDescr(wxTextFile& doc, wxString side);
	wxXmlNode *CreateProductSideDescr(wxString side);
	void PrintFiducial(XmlNodePtrs *pNode, Component comp);
	void PrintComponent(XmlNodePtrs *pNode, Component comp);

	// ��������/���������� ������� � ����
	//
	void LoadProjectInfo(wxString filename);
	void SaveProjectInfo();

	// ������ � ����� ���������
	//
	void LoadComponent(ComponentType& compType, const Component& comp);
	void LoadPattern(Pattern& pattern);

	// ��������� �� ����� � �������� ���������
	//
	bool IsInRoom(double ref1_x, double ref1_y, double ref2_x, double ref2_y, double ptr_x, double ptr_y);

	// ���� ������, ������� ������������
	ComponentVector m_comps;
	ComponentTypeVector m_types;
	PatternVector m_patterns;
	FidMarkVector m_fidmarks;
	SubpcbVector m_pcbs;
	FileTemplate m_fileTempl;

	// ��������� �������
	wxString 	m_project_name;	// ��� ������� (���� �� ������������)
	double		m_height;		// ������� �����
	int			m_angle;  		// ����, ��� ������� ����� ����������� � ������ (0=0, 1=90, 2=180, 3=270)
	double		m_size_x;  		// ������ ������ �����
	double		m_size_y;  		// ������ ������ �����
	double		m_offset_x;		// ���������� �������� �����
	double		m_offset_y;		// ���������� �������� �����
	bool		m_apply_offset;	// ���������� ����������� ��� �������� �� offset - ���� ����������� �������
	wxString 	m_filename;		// ������ ��� ���� �������
	wxArrayString	m_cadfiles;	// � ��� pnp �����
	bool 		m_newProject;

	// ���������
	wxFileConfig *m_settingsCfg;
	wxFileConfig *m_compPcadCfg;
	wxFileConfig *m_compAltumCfg;
	wxFileConfig *m_patternsPcadCfg;
	wxFileConfig *m_patternsAltumCfg;
	// �������� ������
	wxFileConfig *m_activeCompsCfg;
	wxFileConfig *m_activePatternsCfg;
};

wxDECLARE_EVENT(wxEVT_PROJECT_LOADED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PROJECT_CADFILE_LOADED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PROJECT_UPDATED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PROJECT_UPDATE_TITLE, wxCommandEvent);

#define	EVT_PROJECT_LOADED(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_LOADED, func)

#define	EVT_PROJECT_CADFILE_LOADED(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_CADFILE_LOADED, func)

#define	EVT_PROJECT_UPDATED(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_UPDATED, func)

#define	EVT_PROJECT_UPDATE_TITLE(func) \
	EVT_COMMAND(wxID_ANY, wxEVT_PROJECT_UPDATE_TITLE, func)
