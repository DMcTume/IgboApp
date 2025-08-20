/*
* This frames helps set up the ContentMenu by having the user
* choose which part of speech they want to edit within the dictionary.
*/

#pragma once

#ifndef VOCAB_MENU
#define VOCAB_MENU
#include "generic_menu.h"

#define NUM_CONTENT_BUTTONS (4)

class VocabMenu : public GenericMenuFrame {

private:
	
	wxSizerFlags default_flags = wxSizerFlags().Center().Border(wxALL, 2);

	wxButton* noun_button;
	wxButton* verb_button;
	wxButton* descriptor_button;
	wxButton* misc_button;
	wxButton* back_button;

	// Brings user back to the StartingMenu
	void BackToStart(wxCommandEvent& event);

	// Opens up the menu for editing the selected part of speech
	void OpenContent(wxCommandEvent& event);

public:
	VocabMenu();
};
#endif