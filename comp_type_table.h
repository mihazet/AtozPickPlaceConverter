#ifndef COMP_TYPE_TABLE_H
#define COMP_TYPE_TABLE_H

#include <wx/wx.h>
#include <wx/grid.h>

#include "common.h"

class Project;

class ComponentTypeTable : public wxGridTableBase
{
public:
	ComponentTypeTable(Project *project);
	virtual ~ComponentTypeTable();

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
	ComponentTypeVector& m_type;
};

#endif // COMP_TYPE_TABLE_H

