/*
* This menu is used to edit the selected dictionary.
* It is divided into two panels:
* the top panel, which provides multiple different methods of editing,
* and the bottom panel, which facilitates word selection.
*/

#pragma once

#ifndef content_menu
#define content_menu

#include "generic_menu.h"
#include "json_config.h"

#define TOP_PANEL_PROPORTION (3)
#define BOTTOM_PANEL_PROPORTION (7)

#define TOP_BUTTON_WIDTH (120)
#define TOP_BUTTON_HEIGHT (40)

#define TEXT_BOX_WIDTH (450)
#define TEXT_BOX_HEIGHT (40)
#define TEXT_FONT_SCALE (1.1)

#define SEARCH_LIST_WIDTH (450)
#define SEARCH_LIST_HEIGHT (300)

#define NUM_IGBO_SPECIAL_CHARS (4)

class ContentMenu : public GenericMenuFrame {

private: 
	wxSizerFlags default_flags = wxSizerFlags().
		Center().Border(wxALL, 2);

	wxSizerFlags top_panel_flags = wxSizerFlags().
		Proportion(TOP_PANEL_PROPORTION).Border(wxDOWN, 2).Expand();
	
	wxSizerFlags bottom_panel_flags = wxSizerFlags().
		Proportion(BOTTOM_PANEL_PROPORTION).Border(wxUP, 2).Expand();

	wxSizerFlags top_sub_panel_flags = wxSizerFlags().
		Expand().Proportion(1);

	wxSizerFlags search_list_flags = wxSizerFlags().Center().
		Border(wxALL, 2).Proportion(3);

	wxSizerFlags special_char_flags = wxSizerFlags().Center().
		Border(wxALL, 2).Proportion(1);

	// Top Components:
	wxPanel* top_panel;
	wxBoxSizer* top_sizer;

	wxPanel* back_buttons_panel;
	wxPanel* text_box_panel;
	wxPanel* toggle_options_panel;

	wxBoxSizer* back_buttons_sizer;
	wxBoxSizer* text_box_sizer;
	wxBoxSizer* options_sizer;

	wxButton* back_button;
	wxButton* main_menu_button;
	wxButton* add_button;
	wxButton* edit_button;
	wxButton* push_changes_button;
	wxButton* delete_button;

	wxTextCtrl* info_box;

	// Bottom Components: 
	wxPanel* bottom_panel;
	wxBoxSizer* bottom_sizer;

	wxTextCtrl* user_input;
	wxListBox* search_list;

	// Include a captialize button or something to get capital letters
	wxGridSizer* special_char_box;
	wxButton* special_char_buttons[NUM_IGBO_SPECIAL_CHARS];
	wxButton* capitalization_button;
	bool using_uppercase = true;
	
	typedef struct special_char_struct {
		const wchar_t* uppercase;
		const wchar_t* lowercase;
	} special_char;

	map<string, special_char> igbo_special_chars = {
		{"I_UNDERDOT", special_char{L"\u1ECA", L"\u1ECB"}},
		{"O_UNDERDOT", special_char{L"\u1ECC", L"\u1ECD"}},
		{"U_UNDERDOT", special_char{L"\u1EE4", L"\u1EE5"}},
		{"N_NASAL", special_char{L"\u1E44", L"\u1E45"}}
	};

	// Other Non-Component Values

	string category;
	bool dict_update_required; 
	bool changes_made;
	json curr_dict;
	string dict_path = IGBO_DICT_DIR;
	string backup_path = IGBO_BACKUP_DIR;

	/*
	* Returns to VocabMenu (part of speech selection)
	* Automatically pushes changes if needed
	*/
	void GoBackToVocab(wxCommandEvent& event);

	/*
	* Returns to the StartingMenu
	* Automatically pushes changes if needed
	*/
	void GoBackToMainMenu(wxCommandEvent& event);
	
	// Searches dictionary for user's query
	void SearchJSON(wxCommandEvent& event);

	// Presents options for when a word is selected/clicked on
	void SelectWord(wxCommandEvent& event);

	// Add a new word to the dictionary
	void AddWord(wxCommandEvent& event);

	// Edit an existing word in the dictionary
	void EditWord(wxCommandEvent& event);

	// Delete an existing word in the dictionary
	void DeleteWord(wxCommandEvent& event);

	// Save your changes by writing them to the dictionary
	void PushChanges(wxCommandEvent& event);

	void InsertSpecialChar(wxCommandEvent& event);

	void ToggleCaps(wxCommandEvent& event);

public:
	ContentMenu(const char *menu_name, 
		string category,
		json curr_dict);
};

#endif