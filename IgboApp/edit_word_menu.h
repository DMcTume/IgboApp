/*
* This small dialog window allows the user to add
* and edit words in the dictionary.
* It provides fields where they can input their changes.
*/

#pragma once

#ifndef EDIT_WORD_MENU
#define EDIT_WORD_MENU

#include "wx/wx.h"
#include "content_menu.h" // for the special_char struct
#include <string>
#include <map>

using namespace std;

#define WINDOW_WIDTH (300)
#define WINDOW_HEIGHT (350)

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

	wxGridSizer* special_char_grid;
	wxButton* special_char_buttons[NUM_IGBO_SPECIAL_CHARS];
	wxButton* caps_button;
	bool using_uppercase = false;

	// Constructor Params

	wstring* word_name;
	wstring* definition;
	map<string, special_char> special_char_map;
	wxTextCtrl* selected_entry; // for knowing where to insert char

public:

	/*
	* The constructor takes the addresses of the attributes of a word 
	* structure. That word is then added to or changed in the dictionary.
	*/
	EditWordMenu(wstring *word_name, wstring *definition,
		map<string, special_char> *special_char_map);

	// Closes the window with the changes made
	void OnOK(wxCommandEvent& event);

	// Closes the window without making changes
	void OnCancel(wxCommandEvent& event);

	// Closes the window with the changes made
	void SubmitEdit(wxCommandEvent& event);

	void InsertSpecialChar(wxCommandEvent& event);

	void ChangeFocus(wxFocusEvent& event);

	void ToggleCaps(wxCommandEvent& event);
};

#endif