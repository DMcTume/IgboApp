/*
* This is the first menu to be presented upon application 
* start-up.
* It includes options to open other menus and a short
* description of the app.
*/

#pragma once

#ifndef STARTING_MENU
#define STARTING_MENU
#include "generic_menu.h"

class StartingMenu : public GenericMenuFrame {

private:
	
	wxSizerFlags default_flags = wxSizerFlags().Center().Border(wxALL, 2);
	
	wxButton* vocab_options;
	wxButton* practice_options;
	wxButton* app_description;

	// Events:

	// Displays short app description
	void ShowAppDescription(wxCommandEvent& event);

	// Opens frame for editing dictionary
	void OpenVocabFrame(wxCommandEvent& event);
	
	// Opens frame for practicing vocab
	void OpenPracticeMenu(wxCommandEvent& event);

public:
	StartingMenu();
};
#endif