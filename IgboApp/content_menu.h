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

// NOTE: components on the bottom combine above consts and hard-coded ones
// Change that eventually to make it cleaner

#define SEARCH_LIST_WIDTH (450)
#define SEARCH_LIST_HEIGHT (300)

class ContentMenu : public GenericMenuFrame {

	// Think about adding an option to see all current
	// Changes made before pushing

private: 
	wxSizerFlags default_flags = wxSizerFlags().
		Center().Border(wxALL, 2);

	wxSizerFlags top_panel_flags = wxSizerFlags().
		Proportion(TOP_PANEL_PROPORTION).Border(wxDOWN, 2).Expand();
	
	wxSizerFlags bottom_panel_flags = wxSizerFlags().
		Proportion(BOTTOM_PANEL_PROPORTION).Border(wxUP, 2).Expand();

	wxSizerFlags top_sub_panel_flags = wxSizerFlags().
		Expand().Proportion(1);

	// Top Components:
	wxPanel* top_panel;
	wxBoxSizer* top_sizer;

	wxPanel* back_buttons_panel; // NOTE: DEFAULT SIZES MIGHT NEED TO CHANGE
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

	// Other Non-Component Values

	string category;
	bool dict_update_required; 
	json curr_dict;
	string dict_path = IGBO_DICT_DIR;
	string backup_path = IGBO_BACKUP_DIR;

	// Events:
	// Though other methods may edit curr_dict,
	// Changes are only pushed when one of the back buttons are chosen
	// Or when the push change button is selected

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

public:
	ContentMenu(const char *menu_name, 
		string category,
		json curr_dict);
};

#endif