#include "starting_menu_frame.h"

/*
* Runs the app by executing from StartingMenu.
*/

class MainApp : public wxApp {
public: 
	virtual bool OnInit();
};

bool MainApp::OnInit() {

	StartingMenu* frame = new StartingMenu();
	frame->Show(true);
	return true;
}

wxIMPLEMENT_APP(MainApp);

