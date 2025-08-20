#include "content_menu.h"
#include "vocab_menu_frame.h"
#include "starting_menu_frame.h"
#include "edit_word_menu.h"

#include <vector>
#include <cstdio> // needed for remove method

/*
* To solve loading json problem:
* Load json in vocab menu and pass it to this menu
* Then just set the json object equal to this object's attribute
*/

ContentMenu::ContentMenu(const char *menu_name, 
	string category, json curr_dict) : 
	GenericMenuFrame(menu_name, DEFAULT_WIDTH, DEFAULT_HEIGHT) {

	this->category = category;
	this->curr_dict = curr_dict;
	this->dict_update_required = false;

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
		wxSize(SEARCH_LIST_WIDTH, SEARCH_LIST_HEIGHT), 0, NULL,
		wxLB_NEEDED_SB | wxLB_SORT);
	bottom_sizer->Add(search_list, default_flags);

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

void ContentMenu::GoBackToVocab(wxCommandEvent& event) {
	
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

void ContentMenu::GoBackToMainMenu(wxCommandEvent& event) {
	
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

// Functions for ListBox

void ContentMenu::SearchJSON(wxCommandEvent& event) { // Includes binding for edit button

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

		vector<word_t> word_list = dict.at("NOUNS");
		
		string search_word = (string) user_input->GetLineText(0).ToAscii();
		if (search_word.empty()) {
			dict_file.close();
			
			info_box->Clear();
			*info_box << "No input detected";
			return;
		}
		
		vector<word_t> matching_words = {};

		for (word_t word : word_list) {
			if (word.word.find(search_word) != string::npos) {
				matching_words.push_back(word);
			}
		}

		if (matching_words.empty()) {
			wxLogMessage("No matches found!");
		}
		else {
			search_list->Clear();
			for (word_t word : matching_words) {
				search_list->Append(word.word + ": " + word.definition);
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

void ContentMenu::SelectWord(wxCommandEvent& event) {

	edit_button->SetLabel("Edit Word");
	delete_button->SetLabel("Delete Word");
	
	edit_button->Bind(wxEVT_BUTTON, &ContentMenu::EditWord, this, wxID_ANY);
	delete_button->Bind(wxEVT_BUTTON, &ContentMenu::DeleteWord, this, wxID_ANY);
}

// Option Button Functions:

void ContentMenu::AddWord(wxCommandEvent& event) {
	string new_word_name;
	string new_definition;

	EditWordMenu* dialog = new EditWordMenu(&new_word_name, &new_definition);
	if (dialog->ShowModal() == wxID_OK) {
		dialog->Destroy();
	}
	else {
		dialog->Destroy();
	}
	
	word_t new_word = { new_word_name, new_definition, "blank img", "blank ex" };
	
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
}

void ContentMenu::EditWord(wxCommandEvent& event) {

	// Find existing word in json obj
	string selected_string = (string)search_list->GetString(search_list->GetSelection()).ToAscii();
	int parse_marker = selected_string.find(":");
	string selected_word = selected_string.substr(0, parse_marker);
	
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
	EditWordMenu* dialog = new EditWordMenu( &word_info.word, &word_info.definition);
	if (dialog->ShowModal() == wxID_OK) {
		dialog->Destroy();
	}
	else {
		dialog->Destroy();
	}

	curr_dict.at(category)[found_index] = word_info;
}

void ContentMenu::DeleteWord(wxCommandEvent& event) {

	// Find existing word in json obj
	// Has to use an iterator because vector::erase uses it
	string selected_string = (string)search_list->GetString(search_list->GetSelection()).ToAscii();
	int parse_marker = selected_string.find(":");
	string selected_word = selected_string.substr(0, parse_marker);

	vector<word_t> cat_list = curr_dict.at(category);
	word_t word_info;
	vector<word_t>::iterator word_iter;

	for (word_iter = cat_list.begin();
		word_iter != cat_list.end(); word_iter++) {

		if (word_iter->word == selected_word) {
			break;
		}
	}

	// Delete the word

	wxMessageDialog* confirm = new wxMessageDialog(this, 
		"Are you sure you want to permanently delete the selected word?",
		"CAUTION: PERMANENT DELETION CHOSEN", wxYES_NO);

	if (confirm->ShowModal() == wxID_YES) {
		cat_list.erase(word_iter);
		curr_dict.at(category) = cat_list;
	}
	else {
		// pass
	}
	confirm->Destroy();
}

void ContentMenu::PushChanges(wxCommandEvent& event) {
	fstream main_file;
	fstream backup_file;

	wxMessageDialog* confirm = new wxMessageDialog(this,
		"Are you sure you want to push changes to file? ALL CHANGES ARE PERMANENT",
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

