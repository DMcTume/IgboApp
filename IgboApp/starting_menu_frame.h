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

	void ShowAppDescription(wxCommandEvent& event);
	void OpenVocabFrame(wxCommandEvent& event);
	void OpenPracticeMenu(wxCommandEvent& event);

public:
	StartingMenu();
};
#endif