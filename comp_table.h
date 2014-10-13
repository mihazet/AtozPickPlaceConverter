#ifndef COMP_TABLE_H
#define COMP_TABLE_H

#include <wx/wx.h>
#include <wx/grid.h>

#include "common.h"

class Project;

class ComponentTable : public wxGridTableBase
{
public:
	ComponentTable(Project *project);
	virtual ~ComponentTable();

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
	ComponentVector& m_component;
};

#endif // COMP_TABLE_H

