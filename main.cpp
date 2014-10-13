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

