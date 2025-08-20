#pragma once

#ifndef EDIT_WORD_MENU
#define EDIT_WORD_MENU

#include "wx/wx.h"
#include <string>
using namespace std;

#define WINDOW_WIDTH (250)
#define WINDOW_HEIGHT (250)

/*
* Fix this window so that it actually closes
* Look up "wxDialog not closing on exit"
*/

class EditWordMenu : public wxDialog {

private:

	wxSizerFlags default_flags = wxSizerFlags().
		Center().Border(wxALL, 2);

	wxPanel* panel;
	wxBoxSizer* main_sizer;

	wxBoxSizer* word_name_sizer;
	wxBoxSizer* definition_sizer;

	wxStaticText* word_name_text;
	wxTextCtrl* word_name_entry;

	wxStaticText* definition_text;
	wxTextCtrl* definition_entry;

	wxButton* submit_button;

	// Constructor Params

	string* word_name;
	string* definition;

public:

	// Changes the strings passed to it from previous menu
	EditWordMenu(string *word_name, string *definition);

	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	void SubmitEdit(wxCommandEvent& event);
};

#endif