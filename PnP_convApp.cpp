/***************************************************************
 * Name:      PnP_convApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Alatar ()
 * Created:   2012-12-16
 * Copyright: Alatar ()
 * License:
 **************************************************************/

#include "PnP_convApp.h"

//(*AppHeaders
#include "PnP_convMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PnP_convApp);

bool PnP_convApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    PnP_convFrame* Frame = new PnP_convFrame(0);
    Frame->Show();
    SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
