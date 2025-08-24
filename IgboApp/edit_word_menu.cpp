#include "edit_word_menu.h"

#include <sstream>

EditWordMenu::EditWordMenu(string *word_name, string *definition,
	map<string, special_char> *special_char_map) : 
	wxDialog(NULL, wxID_ANY, (*word_name).c_str()) {

	// Main Components:
	this->word_name = word_name;
	this->definition = definition;
	this->special_char_map = *special_char_map;
	this->SetSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));

	panel = new wxPanel(this);
	panel->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));
	main_sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(main_sizer);
	main_sizer->SetSizeHints(panel);

	submit_button = new wxButton(panel, wxID_ANY, "SUBMIT EDIT");

	// Sub-Sizers:

	word_name_sizer = new wxBoxSizer(wxHORIZONTAL);
	definition_sizer = new wxBoxSizer(wxHORIZONTAL);

	word_name_text = new wxStaticText(panel, wxID_ANY, "Word: ",
		wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);

	word_name_entry = new wxTextCtrl(panel, wxID_ANY, *word_name,
		wxDefaultPosition, wxDefaultSize);
	word_name_entry->Bind(wxEVT_SET_FOCUS, &EditWordMenu::ChangeFocus, this);
	word_name_entry->SetFocus();

	definition_text = new wxStaticText(panel, wxID_ANY, "Definition: ",
		wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);

	definition_entry = new wxTextCtrl(panel, wxID_ANY, *definition,
		wxDefaultPosition, wxDefaultSize);
	definition_entry->Bind(wxEVT_SET_FOCUS, &EditWordMenu::ChangeFocus, this);

	// BoxSizer stuff

	word_name_sizer->Add(word_name_text, default_flags);
	word_name_sizer->Add(word_name_entry, default_flags);

	definition_sizer->Add(definition_text, default_flags); 
	definition_sizer->Add(definition_entry, default_flags);

	main_sizer->Add(word_name_sizer, default_flags);
	main_sizer->Add(definition_sizer, default_flags);
	main_sizer->Add(submit_button, default_flags);

	submit_button->Bind(wxEVT_BUTTON, &EditWordMenu::SubmitEdit, this, wxID_ANY);

	// Special Chars:

	special_char_grid = new wxGridSizer(NUM_IGBO_SPECIAL_CHARS);
	main_sizer->Add(special_char_grid, default_flags);

	int button_index = 0;
	for (auto it = this->special_char_map.begin(); it != this->special_char_map.end();
		it++) {
		const wchar_t* letter = it->second.lowercase;
		special_char_buttons[button_index] = new wxButton(panel,
			wxID_ANY, letter);
		special_char_buttons[button_index]->Bind(wxEVT_BUTTON,
			&EditWordMenu::InsertSpecialChar, this);
		main_sizer->Add(special_char_buttons[button_index], default_flags);
		button_index++;
	}

	caps_button = new wxButton(panel, wxID_ANY, "Toggle Caps");
	main_sizer->Add(caps_button, default_flags);
	caps_button->Bind(wxEVT_BUTTON, &EditWordMenu::ToggleCaps, this);
}

void EditWordMenu::OnOK(wxCommandEvent& event) {
	EndModal(wxID_OK);
}

void EditWordMenu::OnCancel(wxCommandEvent& event) {
	EndModal(wxID_CANCEL);
}

void EditWordMenu::SubmitEdit(wxCommandEvent& event) {
	
	string submitted_name = (string) word_name_entry->GetLineText(0).ToAscii();
	string submitted_definition = (string) definition_entry->GetLineText(0).ToAscii();

	*word_name = submitted_name;
	*definition = submitted_definition;

	EndModal(wxID_OK);
}


// FIX CURSOR PROBLEM BOTH HERE AND IN CONTENT MENU
// might involve having focus instantly change back to the ctrl
// then placing the cursor at the end of the string

void EditWordMenu::InsertSpecialChar(wxCommandEvent& event) {
	wxString char_selected = ((wxButton*)event.GetEventObject())->GetLabel();
	*selected_entry << char_selected;
	selected_entry->SetFocus();
}

void EditWordMenu::ChangeFocus(wxFocusEvent& event) {
	selected_entry = (wxTextCtrl*)event.GetEventObject();
}

void EditWordMenu::ToggleCaps(wxCommandEvent& event) {
	int button_index = 0;

	for (const auto& pair : special_char_map) {
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
	/*stringstream ss;
	for (wxButton* button : special_char_buttons) {
		ss << static_cast<void*>(button) << "\n";
	}
	wxLogMessage(ss.str());*/
}