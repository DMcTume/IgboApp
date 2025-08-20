#include "edit_word_menu.h"

EditWordMenu::EditWordMenu(string *word_name, string *definition) : 
	wxDialog(NULL, wxID_ANY, (*word_name).c_str()) {

	// Main Components:
	this->word_name = word_name;
	this->definition = definition;
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

	definition_text = new wxStaticText(panel, wxID_ANY, "Definition: ",
		wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);

	definition_entry = new wxTextCtrl(panel, wxID_ANY, *definition,
		wxDefaultPosition, wxDefaultSize);

	// BoxSizer stuff

	word_name_sizer->Add(word_name_text, default_flags);
	word_name_sizer->Add(word_name_entry, default_flags);

	definition_sizer->Add(definition_text, default_flags); 
	definition_sizer->Add(definition_entry, default_flags);

	main_sizer->Add(word_name_sizer, default_flags);
	main_sizer->Add(definition_sizer, default_flags);
	main_sizer->Add(submit_button, default_flags);

	submit_button->Bind(wxEVT_BUTTON, &EditWordMenu::SubmitEdit, this, wxID_ANY);
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