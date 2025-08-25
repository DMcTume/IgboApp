#include "content_menu.h"
#include "vocab_menu_frame.h"
#include "starting_menu_frame.h"
#include "edit_word_menu.h"

#include <vector>
#include <cstdio>


ContentMenu::ContentMenu(const char *menu_name, 
	string category, json curr_dict) : 
	GenericMenuFrame(menu_name, DEFAULT_WIDTH, DEFAULT_HEIGHT) {

	this->category = category;
	this->curr_dict = curr_dict;
	this->dict_update_required = false;
	this->changes_made = false;

	// Main Top Panel
	
	this->SetMinSize(wxSize(845, 520));
	getPanel()->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));

	top_panel = new wxPanel(getPanel(), wxID_ANY, wxDefaultPosition, 
		wxDefaultSize);

	top_sizer = new wxBoxSizer(wxVERTICAL);
	top_panel->SetSizer(top_sizer);
	top_sizer->SetSizeHints(getPanel());

	// Top Panel: BACK BUTTONS

	back_buttons_panel = new wxPanel(top_panel, wxID_ANY, wxDefaultPosition,
		wxSize(20, 20));
	top_sizer->Add(back_buttons_panel, top_sub_panel_flags);

	back_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	back_buttons_panel->SetSizer(back_buttons_sizer);
	back_buttons_sizer->SetSizeHints(top_panel);

	// Top Panel: TEXT BOX

	text_box_panel = new wxPanel(top_panel, wxID_ANY, wxDefaultPosition,
		wxSize(20, 20));
	top_sizer->Add(text_box_panel, top_sub_panel_flags);

	text_box_sizer = new wxBoxSizer(wxHORIZONTAL);
	text_box_panel->SetSizer(text_box_sizer);
	text_box_sizer->SetSizeHints(top_panel);

	// Top Panel: TOGGLE OPTIONS

	toggle_options_panel = new wxPanel(top_panel, wxID_ANY, wxDefaultPosition,
		wxSize(20, 20));
	top_sizer->Add(toggle_options_panel, top_sub_panel_flags);

	options_sizer = new wxBoxSizer(wxHORIZONTAL);
	toggle_options_panel->SetSizer(options_sizer);
	options_sizer->SetSizeHints(top_panel);

	// Main Bottom Panel

	bottom_panel = new wxPanel(getPanel(), wxID_ANY, wxDefaultPosition, 
		wxDefaultSize);
	bottom_sizer = new wxBoxSizer(wxVERTICAL);
	bottom_panel->SetSizer(bottom_sizer);
	bottom_sizer->SetSizeHints(getPanel());

	/* /// SUB PANEL CONTROLS DEFINED BELOW /// */

	// TOP: BACK BUTTONS (spacers used for horizontal alignment)

	back_buttons_sizer->AddStretchSpacer();
	back_button = makeMenuButton(back_buttons_panel, back_buttons_sizer, wxID_ANY, 
		"Return to Vocab", default_flags);
	back_button->SetMinSize(wxSize(TOP_BUTTON_WIDTH, TOP_BUTTON_HEIGHT));

	main_menu_button = makeMenuButton(back_buttons_panel, back_buttons_sizer,
		wxID_ANY, "Return to Main Menu", default_flags);
	main_menu_button->SetMinSize(wxSize(TOP_BUTTON_WIDTH, TOP_BUTTON_HEIGHT));
	back_buttons_sizer->AddStretchSpacer();

	// TOP: TEXT BOX 

	info_box = new wxTextCtrl(text_box_panel, wxID_ANY, 
		"Type in the text box below, and press ENTER to search for a word",
		wxDefaultPosition, wxSize(0,0), wxTE_READONLY | wxTE_CENTER);
	info_box->SetFont(info_box->GetFont().Scale(TEXT_FONT_SCALE));
	info_box->SetMinSize(wxSize(TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT));
	text_box_sizer->AddStretchSpacer();
	text_box_sizer->Add(info_box, wxSizerFlags().Center());
	text_box_sizer->AddStretchSpacer();

	// TOP: OPTION BUTTONS (spacers used for horizontal alignment)

	wxButton** buttons[] = { &add_button, &edit_button, &push_changes_button,
		&delete_button};

	options_sizer->AddStretchSpacer();
	for (int i = 0; i < 4; i++) {
		*buttons[i] = makeMenuButton(toggle_options_panel, options_sizer,
			wxID_ANY, NULL, default_flags);
		(*buttons[i])->SetMinSize(wxSize(TOP_BUTTON_WIDTH, TOP_BUTTON_HEIGHT*0.75));
	}
	options_sizer->AddStretchSpacer();

	add_button->SetLabel("Add New Word");
	push_changes_button->SetLabel("Push Changes");

	// BOTTOM SUB-PANELS

	user_input = new wxTextCtrl(bottom_panel, wxID_ANY, "Search for word here",
		wxDefaultPosition, wxSize(TEXT_BOX_WIDTH, 25), wxTE_PROCESS_ENTER);
	bottom_sizer->Add(user_input, default_flags);

	search_list = new wxListBox(bottom_panel, wxID_ANY, wxDefaultPosition,
		wxSize(SEARCH_LIST_WIDTH, -1), 0, NULL,
		wxLB_NEEDED_SB | wxLB_SORT);
	bottom_sizer->Add(search_list, search_list_flags);

	special_char_box = new wxGridSizer(NUM_IGBO_SPECIAL_CHARS);
	bottom_sizer->Add(special_char_box, special_char_flags);

	int button_index = 0;
	for (const auto& pair : igbo_special_chars) {
		special_char_buttons[button_index] = new wxButton(bottom_panel, wxID_ANY, pair.second.lowercase);
		special_char_buttons[button_index]->Bind(wxEVT_BUTTON, &ContentMenu::InsertSpecialChar, this);
		special_char_box->Add(special_char_buttons[button_index]);
		button_index++;
	}

	capitalization_button = new wxButton(bottom_panel, wxID_ANY, "Toggle Capitalization");
	capitalization_button->Bind(wxEVT_BUTTON, &ContentMenu::ToggleCaps, this);
	bottom_sizer->Add(capitalization_button, default_flags);

	getBoxSizer()->Add(top_panel, top_panel_flags);
	getBoxSizer()->Add(bottom_panel, bottom_panel_flags);

	// EVENT BINDING: 

	back_button->Bind(wxEVT_BUTTON, &ContentMenu::GoBackToVocab, this, wxID_ANY);
	main_menu_button->Bind(wxEVT_BUTTON, &ContentMenu::GoBackToMainMenu, this, wxID_ANY);
	user_input->Bind(wxEVT_TEXT_ENTER, &ContentMenu::SearchJSON, this, wxID_ANY);
	search_list->Bind(wxEVT_LISTBOX, &ContentMenu::SelectWord, this, wxID_ANY);

	push_changes_button->Bind(wxEVT_BUTTON, &ContentMenu::PushChanges, this, wxID_ANY);
	add_button->Bind(wxEVT_BUTTON, &ContentMenu::AddWord, this, wxID_ANY);
}

/*
* Brings user back to the VocabMenu, but first asks if they want
* to save their current changes.
*/
void ContentMenu::GoBackToVocab(wxCommandEvent& event) {
	
	if (changes_made) {
		wxMessageDialog* exit_check = new wxMessageDialog(this, "Are you sure you want to exit without pushing changes?",
			"CHANGES UNSAVED", wxYES_NO);

		if (exit_check->ShowModal() == wxID_YES) {
			int x, y;
			this->GetPosition(&x, &y);

			VocabMenu* frame = new VocabMenu();
			frame->SetPosition(wxPoint(x, y));
			frame->Show();
			this->Destroy();
		}
		else {
			// pass
		}
		exit_check->Destroy();
	}
	else {
		int x, y;
		this->GetPosition(&x, &y);

		VocabMenu* frame = new VocabMenu();
		frame->SetPosition(wxPoint(x, y));
		frame->Show();
		this->Destroy();
	}
}

/*
* Brings user back to the StartingMenu, but first asks if they want
* to save their current changes.
*/
void ContentMenu::GoBackToMainMenu(wxCommandEvent& event) {
	
	if (changes_made) {
		wxMessageDialog* exit_check = new wxMessageDialog(this, "Are you sure you want to exit without pushing changes?",
			"CHANGES UNSAVED", wxYES_NO);

		if (exit_check->ShowModal() == wxID_YES) {
			int x, y;
			this->GetPosition(&x, &y);

			StartingMenu* frame = new StartingMenu();
			frame->SetPosition(wxPoint(x, y));
			frame->Show();
			this->Destroy();
		}
		else {
			// pass
		}
		exit_check->Destroy();
	}
	else {
		int x, y;
		this->GetPosition(&x, &y);

		StartingMenu* frame = new StartingMenu();
		frame->SetPosition(wxPoint(x, y));
		frame->Show();
		this->Destroy();
	}
}

// Functions for ListBox

/*
* First loads updates the menu's copy of the dictionary if needed.
* It then presents all words that contain the user's searched word.
* It also disables some of the top panel methods, specfically
* the ones that require a word to be selected (because this method resets
* selection).
*/
void ContentMenu::SearchJSON(wxCommandEvent& event) {

	fstream dict_file;
	json dict;

	try {
		if (dict_update_required) {
			if (open_JSON(&dict_file, IGBO_DICT_DIR, 'R')) {
				throw new runtime_error("Could not open dictionary file!");
			}

			dict = json::parse(dict_file);
			curr_dict = dict;
			dict_update_required = false;
		}
		else {
			dict = curr_dict;
		}

		vector<word_t> word_list = dict.at(category);
		
		// wxString search_word = user_input->GetLineText(0); 
		wxString search_word = user_input->GetValue();
		if (search_word.empty()) {
			dict_file.close();
			
			info_box->Clear();
			*info_box << "No input detected";
			return;
		}
		
		vector<word_t> matching_words = {};

		for (word_t word : word_list) {
			if (word.word.find(search_word) != wstring::npos) {
				matching_words.push_back(word);
			}
		}

		if (matching_words.empty()) {
			wxLogMessage("No matches found!");
		}
		else {
			search_list->Clear();
			for (word_t word : matching_words) {
				search_list->Append(word.word + wxString(": ") + word.definition);
			}
		}

		info_box->Clear();
		*info_box << "Found " << to_string(matching_words.size()) << " results";

		dict_file.close();

		// Clear selection
		search_list->SetSelection(wxNOT_FOUND);
		edit_button->SetLabel("");
		delete_button->SetLabel("");
		edit_button->Unbind(wxEVT_BUTTON, &ContentMenu::EditWord, this, wxID_ANY);
		delete_button->Unbind(wxEVT_BUTTON, &ContentMenu::DeleteWord, this, wxID_ANY);
	}
	catch (const exception& e) {
		wxLogMessage(e.what());
		
		if (dict_file) {
			dict_file.close();
		}
	}
}

/*
* This method enables top-panel methods that require a word to 
* be selected once a user clicks on a word.
*/
void ContentMenu::SelectWord(wxCommandEvent& event) {

	edit_button->SetLabel("Edit Word");
	delete_button->SetLabel("Delete Word");
	
	edit_button->Bind(wxEVT_BUTTON, &ContentMenu::EditWord, this, wxID_ANY);
	delete_button->Bind(wxEVT_BUTTON, &ContentMenu::DeleteWord, this, wxID_ANY);
}

// Option Button Functions:

/*
* This method brings up the EditWordMenu so the user can create the 
* word they want to add.
* It then adds the new word to the menu's current copy of the dictionary.
* This method does not allow words of the same name to be added.
* (Other attributes, such as definition, can be matching, however)
*/
void ContentMenu::AddWord(wxCommandEvent& event) {
	wstring new_word_name;
	wstring new_definition;

	EditWordMenu* dialog = new EditWordMenu(&new_word_name, &new_definition,
		&igbo_special_chars);
	if (dialog->ShowModal() == wxID_OK) {
		dialog->Destroy();
		word_t new_word = { new_word_name, new_definition, "blank img", utf8_to_wstring("blank ex") };

		vector<word_t> word_list = curr_dict.at(category);
		int new_index = word_list.size();

		// Search for duplicates before adding

		for (word_t word : word_list) {
			if (word.word == new_word_name) {
				wxLogMessage("Word already found in list!");
				return;
			}
		}

		curr_dict.at(category)[new_index] = new_word;
		changes_made = true;
	}
	else {
		dialog->Destroy();
	}
}

/*
* This method brings up the EditWordMenu so the user can change the
* attributes of an existing word.
* It does this by editing the menu's copy of the dictionary.
* If a word is edited, it cannot have the same name as another word
* already in the dictionary.
*/
void ContentMenu::EditWord(wxCommandEvent& event) {

	// Find existing word in json obj
	wxString selected_string = search_list->GetString(search_list->GetSelection());
	int parse_marker = selected_string.find(":");
	wxString selected_word = selected_string.substr(0, parse_marker);
	
	vector<word_t> cat_list = curr_dict.at(category);
	word_t word_info;
	int found_index = -1;

	for (int i = 0; i < cat_list.size(); i++) {
		if (cat_list.at(i).word == selected_word) {
			word_info = cat_list.at(i);
			found_index = i;
		}
	}

	// Have user input from dialog directly change the struct
	EditWordMenu* dialog = new EditWordMenu( &word_info.word, &word_info.definition,
		&igbo_special_chars);
	if (dialog->ShowModal() == wxID_OK) {
		dialog->Destroy();
		
		// Make sure their edit didn't create a duplicate
		boolean duplicate_found = false;
		for (word_t word : cat_list) {
			if (word.word == word_info.word) {
				duplicate_found = true;
			}
		}

		if (duplicate_found) {
			wxLogMessage("A word already has this name, try again!");
		}
		else {
			search_list->Delete(found_index);
			search_list->Append(word_info.word + wxString(": ") + word_info.definition);
			curr_dict.at(category)[found_index] = word_info;

			*info_box << "Edited word was placed at the bottom of the list!";
			changes_made = true;
		}
	}
	else {
		dialog->Destroy();
	}
}

/*
* This method deletes the selected word from the menu's copy
* of the dictionary.
*/
void ContentMenu::DeleteWord(wxCommandEvent& event) {

	// Find existing word in json obj
	// Has to use an iterator because vector::erase uses it
	wxString selected_string = search_list->GetString(search_list->GetSelection());
	int parse_marker = selected_string.find(":");
	wxString selected_word = selected_string.substr(0, parse_marker);

	vector<word_t> cat_list = curr_dict.at(category);
	word_t word_info;
	vector<word_t>::iterator word_iter;
	int word_index = -1;

	for (word_iter = cat_list.begin();
		word_iter != cat_list.end(); word_iter++) {
		word_index++;
		if (word_iter->word == selected_word) {
			break;
		}
	}

	// Delete the word

	wxMessageDialog* confirm = new wxMessageDialog(this, 
		"Are you sure you want to permanently delete the selected word?",
		"CAUTION: PERMANENT DELETION CHOSEN", wxYES_NO);

	if (confirm->ShowModal() == wxID_YES) {
		search_list->Delete(word_index);
		cat_list.erase(word_iter);
		curr_dict.at(category) = cat_list;
		changes_made = true;
	}
	else {
		// pass
	}
	confirm->Destroy();
}

/*
* This method saves the user's changes by writing the menu's current
* copy of the dictionary to the original dictionary.
* Before attempting to write, it creates a backup. If there is an error,
* the erronenous dictionary (the original) will be deleted and replaced
* with the backup.
* This effectively means that upon error, the user's changes will not be saved
* (the used will be notified of this).
*/
void ContentMenu::PushChanges(wxCommandEvent& event) {
	fstream main_file;
	fstream backup_file;

	if (!changes_made) {
		wxLogMessage("You've yet to make any changes!");
		return;
	}

	wxMessageDialog* confirm = new wxMessageDialog(this,
		"Are you sure you want to push changes to file? ALL CHANGES ARE PERMANENT\n"
		"Additionally, your changes will not be saved upon error",
		"CAUTION: PERMANENT CHANGES CHOSEN", wxYES_NO);

	if (confirm->ShowModal() == wxID_YES) {
		// pass;
	}
	else {
		confirm->Destroy();
		return;
	}
	confirm->Destroy();

	try {

		if (create_backup(&main_file, &backup_file,
			dict_path, backup_path)) {
			main_file.close();
			throw new runtime_error("Could not create backup for writing!");
		}

		if (open_JSON(&main_file, dict_path, 'W')) {
			throw new runtime_error("Could not open or create file for writing!");
		}

		main_file << curr_dict.dump(2);
		
		backup_file.close();
		remove(backup_path.c_str());
		main_file.close();
		wxLogMessage("Successfully wrote changes!");
		changes_made = false;
	}
	catch (const exception& e) {
		wxLogMessage(e.what());

		// Back up fails
		if (e.what() == "Could not create backup for writing!") {
			return;
		}

		// Opening main file stream fails
		else if (e.what() == "Could not open or create file for writing!") {
			backup_file.close();
		}

		// Should address parse/writing errors 
		else {
			main_file.close();
			backup_file.close();
			remove(dict_path.c_str());
			rename(backup_path.c_str(), dict_path.c_str());
		}
	}
}


void ContentMenu::InsertSpecialChar(wxCommandEvent& event) {
	wxString char_selected = ((wxButton*)event.GetEventObject())->GetLabel();
	wxString user_input_str = user_input->GetValue();

	if (user_input_str == "Search for word here") {
		user_input->Clear();
	}
	*user_input << char_selected;
	user_input->SetFocus(); 
}

void ContentMenu::ToggleCaps(wxCommandEvent& event) {
	int button_index = 0;

	for (const auto& pair : igbo_special_chars) {
		if (using_uppercase) {
			special_char_buttons[button_index]->SetLabel(pair.second.lowercase);
		}
		else {
			special_char_buttons[button_index]->SetLabel(pair.second.uppercase);
		}
		button_index++;
	}

	if (using_uppercase) {
		using_uppercase = false;
	}
	else {
		using_uppercase = true;
	}
}
