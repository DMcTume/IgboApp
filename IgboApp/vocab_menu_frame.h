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

	// Events:
	void BackToStart(wxCommandEvent& event);
	void OpenContent(wxCommandEvent& event);

public:
	VocabMenu();
};
#endif