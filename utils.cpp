#include <wx/regex.h>
#include <wx/numformatter.h>


// helper function
// from Alatar's project without modifications
wxString RemoveQuotes(const wxString& s)
{
	wxString str = s;
	wxUniChar quote_char = s[0];

	if (((quote_char != '\'') && (quote_char != '"')) || (s[s.Len() - 1] != quote_char))
	{
		return str.Trim(true).Trim(false);
	}
	str = s.Mid(1, s.Len() - 2);
	str = str.Trim(true).Trim(false);
	return str;
}

// helper function
// from Alatar's project without modifications
wxString ParseNominal(wxString a_designator, wxString a_value)
{
	char ch;
	double tmp_val;
	double factor = 1;
	wxString val, tmp_unit;

	double		value = -1;		// Номинальное значение для резисторов/еондёров/etc.
	wxString	unit;			// Размерность номинального значения
	wxString	value_postfix;	// Утточнение номинала (например вольтаж кондёра)
	wxString	result;			// Утточнение номинала (например вольтаж кондёра)

	ch = a_designator.Upper()[0];
	switch (ch)
	{
		case 'R':
			unit = "";
			break;
		case 'C':
			unit = "F";
			factor = 1e-6;
			break;
		case 'L':
			unit = "H";
			break;
		default:
			return a_value;
	}
	//wxLogVerbose(_T(">> Input is %s for %s"), a_value, a_designator);

	wxRegEx re_format1("^([[:digit:]]+)([pnumkKMrRfFhH]{1,2})([[:digit:]]+)(.*)$"); //1p1F bla-bla-bla
	wxRegEx re_format2("^([[:digit:]]+[.,]?[[:digit:]]*)([pnumkKM]?)[fFhH]?(.*)$"); //1.1pF bla-bla-bla
	if (re_format1.Matches(a_value))
	{
		wxString tmp_str = re_format1.GetMatch(a_value, 1) + "." + re_format1.GetMatch(a_value, 3) + re_format1.GetMatch(a_value, 2) + re_format1.GetMatch(a_value, 4);
		a_value = tmp_str;
	}
	if (!re_format2.Matches(a_value))
	{
		value = -1;
		return wxEmptyString;
	}
	val = re_format2.GetMatch(a_value, 1); val.Replace(",", ".");
	tmp_unit = re_format2.GetMatch(a_value, 2);
	value_postfix = re_format2.GetMatch(a_value, 3);

	//wxLogVerbose(_T(">> Input is %s, Val is %s, Unit is %s, Postfix is %s"), a_value, val, tmp_unit, value_postfix);

	if ((!val.ToCDouble(&tmp_val)) || (tmp_val < 0))
	{
		value = -1;
		//wxLogVerbose(_T("Convert fail! Val is %s, converted val is %f"), val, tmp_val);
		return wxEmptyString;
	}
	value = tmp_val;

	ch = tmp_unit[0];
	switch (ch)
	{
		case 'p':
			factor = 1e-12;
			break;
		case 'n':
			factor = 1e-9;
			break;
		case 'u':
			factor = 1e-6;
			break;
		case 'm':
			factor = 1e-3;
			break;
		case 'k':
		case 'K':	//кривое название, но встречается часто
			factor = 1e3;
			break;
		case 'M':
			factor = 1e6;
			break;
	}
	//wxLogVerbose(_T("Scale: Val is %f, factor is %f"), value, factor);
	value *= factor;

	//wxLogVerbose(_T("PnP_convFrame::NormalizeNominal %f - %s"), value, unit);
	if (value == 0)
	{
		//wxLogVerbose(_T("PnP_convFrame::NormalizeNominal Zero on input: %f - %s"), value, unit);
	}
	else if (value < 1e-9)
	{
		value /= 1e-12;
		unit.Prepend("p");
		//wxLogVerbose(_T("Step 1e-9 -> %f - %s"), value, unit);
	}
	else if (value < 1e-6)
	{
		if ("F" == unit)
		{
			if (value < 1e-8)
			{
				value /= 1e-12;
				unit.Prepend("p");
			}
			else
			{
				value /= 1e-6;
				unit.Prepend("u");
			}
		}
		else
		{
			value /= 1e-9;
			unit.Prepend("n");
		}
		//wxLogVerbose(_T("Step 1e-6 -> %f - %s"), value, unit);
	}
	else if (value < 1e-3)
	{
		value /= 1e-6;
		unit.Prepend("u");
		//wxLogVerbose(_T("Step 1e-3 -> %f - %s"), value, unit);
	}
	else if (value < 1)
	{
		if (("F" == unit) && (value < 1e-1))
		{
			value /= 1e-6;
			unit.Prepend("u");
		}
		else
		{
			value /= 1e-3;
			unit.Prepend("m");
		}
		//wxLogVerbose(_T("Step 1e-0 -> %f - %s"), value, unit);
	}
	else if (value < 1e3)
	{
		//		value /= 1e0;
		//		unit.Prepend("");
		//wxLogVerbose(_T("Step 1e3 -> %f - %s"), value, unit);
	}
	else if (value < 1e6)
	{
		value /= 1e3;
		unit.Prepend("k");
		//wxLogVerbose(_T("Step 1e6 -> %f - %s"), value, unit);
	}
	else
	{
		value /= 1e6;
		unit.Prepend("M");
		//wxLogVerbose(_T("Step 1e9 -> %f - %s"), value, unit);
	}

	result = wxNumberFormatter::ToString(value, 8, wxNumberFormatter::Style_NoTrailingZeroes)/*.Replace(",", ".")*/ + unit + value_postfix;
	//wxLogVerbose(_T("<< Result is %s"), result);
	return result;
}


