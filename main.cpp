/***************************************************************
 * Name:      main.cpp
 * Purpose:   Code for Application Class
 * Author:    Alatar ()
 * Created:   2012-12-16
 * Copyright: Alatar ()
 * License:
 **************************************************************/

#include <wx/wx.h>
#include "main_frame.h"

class App : public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(App)

bool App::OnInit()
{
	MainFrame *frame = new MainFrame();
	frame->Show();

	wxLogMessage("Program started.");

	return true;
}

