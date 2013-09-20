#ifndef PATTERN_TABLE_H
#define PATTERN_TABLE_H

#include "common.h"
#include <wx/wx.h>
#include <wx/grid.h>

class wxFileConfig;

class cPatternTable : public wxGridTableBase
{
public:
	cPatternTable(tPatternDescr *a_data);
	virtual ~cPatternTable();
	void SetPatternsConfig(wxFileConfig *a_config) {m_config = a_config;};

	// You must override these functions in a derived table class
	//

	// return the number of rows and columns in this table
	virtual int GetNumberRows();
	virtual int GetNumberCols();
//	virtual bool IsEmptyCell( int row, int col )
//	{
////		return GetValue(row, col).empty();
//		return false;
//	}

	virtual wxString GetValue( int a_row, int a_col );
	virtual void SetValue( int a_row, int a_col, const wxString& a_value );

//	// Data type determination and value access
//	virtual wxString GetTypeName( int row, int col );
//	virtual bool CanGetValueAs( int row, int col, const wxString& typeName );
//	virtual bool CanSetValueAs( int row, int col, const wxString& typeName );
//
//	virtual long GetValueAsLong( int row, int col );
//	virtual double GetValueAsDouble( int row, int col );
//	virtual bool GetValueAsBool( int row, int col );
//
//	virtual void SetValueAsLong( int row, int col, long value );
//	virtual void SetValueAsDouble( int row, int col, double value );
//	virtual void SetValueAsBool( int row, int col, bool value );
//
//	// For user defined types
//	virtual void* GetValueAsCustom( int row, int col, const wxString& typeName );
//	virtual void  SetValueAsCustom( int row, int col, const wxString& typeName, void* value );
//
//
//	// Overriding these is optional
//	//
//	virtual void SetView( wxGrid *grid ) { m_view = grid; }
//	virtual wxGrid * GetView() const { return m_view; }
//
//	virtual void Clear() {}
//	virtual bool InsertRows( size_t pos = 0, size_t numRows = 1 );
//	virtual bool AppendRows( size_t numRows = 1 );
//	virtual bool DeleteRows( size_t pos = 0, size_t numRows = 1 );
//	virtual bool InsertCols( size_t pos = 0, size_t numCols = 1 );
//	virtual bool AppendCols( size_t numCols = 1 );
//	virtual bool DeleteCols( size_t pos = 0, size_t numCols = 1 );
//
//	virtual wxString GetRowLabelValue( int row );
	virtual wxString GetColLabelValue( int a_col );
//	virtual void SetRowLabelValue( int WXUNUSED(row), const wxString& ) {}
//	virtual void SetColLabelValue( int WXUNUSED(col), const wxString& ) {}
//
//	// Attribute handling
//	//
//
//	// give us the attr provider to use - we take ownership of the pointer
//	void SetAttrProvider(wxGridCellAttrProvider *attrProvider);
//
//	// get the currently used attr provider (may be NULL)
//	wxGridCellAttrProvider *GetAttrProvider() const { return m_attrProvider; }
//
//	// Does this table allow attributes?  Default implementation creates
//	// a wxGridCellAttrProvider if necessary.
//	virtual bool CanHaveAttributes();
//
//	// by default forwarded to wxGridCellAttrProvider if any. May be
//	// overridden to handle attributes directly in the table.
//	virtual wxGridCellAttr *GetAttr( int row, int col,
//					wxGridCellAttr::wxAttrKind  kind );
//
//
//	// these functions take ownership of the pointer
//	virtual void SetAttr(wxGridCellAttr* attr, int row, int col);
//	virtual void SetRowAttr(wxGridCellAttr *attr, int row);
//	virtual void SetColAttr(wxGridCellAttr *attr, int col);

protected:
private:
	tPatternDescr		*m_pattern_data;
	wxFileConfig		*m_config;
};

#endif // EVENTS_TABLE_H
