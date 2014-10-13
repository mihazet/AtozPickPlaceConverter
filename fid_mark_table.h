#ifndef FID_MARK_TABLE_H
#define FID_MARK_TABLE_H

#include <wx/wx.h>
#include <wx/grid.h>

#include "common.h"

class Project;

class FidMarkTable : public wxGridTableBase
{
public:
	FidMarkTable(Project *project);
	virtual ~FidMarkTable();

	// You must override these functions in a derived table class
	//

	// return the number of rows and columns in this table
	virtual int GetNumberRows();
	virtual int GetNumberCols();
//	virtual bool IsEmptyCell( int row, int col ) { return false; }
	virtual wxString GetValue( int row, int col );
	virtual void SetValue( int row, int col, const wxString& value );

	virtual wxString GetColLabelValue( int col );

	// Сортировка
	// col - номер столбца, по которому происходит сортировка
	void Sort(int col);

private:
	Project *m_project;
	FidMarkVector& m_fidmark;
	ComponentVector& m_component;
};

#endif // FID_MARK_TABLE_H

