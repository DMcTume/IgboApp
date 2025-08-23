/*
* This small dialog window allows the user to add
* and edit words in the dictionary.
* It provides fields where they can input their changes.
*/

#pragma once

#ifndef EDIT_WORD_MENU
#define EDIT_WORD_MENU

#include "wx/wx.h"
#include <string>
#include <map>

using namespace std;

#define WINDOW_WIDTH (250)
#define WINDOW_HEIGHT (250)

#define UPPERCASE (0)
#define LOWERCASE (1)

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

	/*
	* The constructor takes the addresses of the attributes of a word 
	* structure. That word is then added to or changed in the dictionary.
	*/
	//EditWordMenu(string *word_name, string *definition,
	//	map<string, WordValue> *special_chars);

	EditWordMenu(string *word_name, string *definition);

	// Closes the window with the changes made
	void OnOK(wxCommandEvent& event);

	// Closes the window without making changes
	void OnCancel(wxCommandEvent& event);

	// Closes the window with the changes made
	void SubmitEdit(wxCommandEvent& event);
};

#endif