#include "dict_view.h"

DictView::DictView(const char* menu_name, vector<word_t> word_list): 
	wxDialog(NULL, wxID_ANY, menu_name) {

	this->word_list = word_list;
	this->main_panel = new wxPanel(this);
	main_panel->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));

	this->main_sizer = new wxBoxSizer(wxVERTICAL);
	main_panel->SetSizer(main_sizer);
	main_sizer->SetSizeHints(main_panel);

	word_list_box = new wxListBox(main_panel, wxID_ANY, wxDefaultPosition,
		wxSize(LIST_WIDTH, LIST_HEIGHT));
	main_sizer->Add(word_list_box, default_flags);

	button_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(button_sizer, default_flags);

	prev_page = new wxButton(main_panel, wxID_ANY, "");
	next_page = new wxButton(main_panel, wxID_ANY, "");

	button_sizer->Add(prev_page, default_flags);
	button_sizer->Add(next_page, default_flags);

	// Fill up the ListBox box

	word_idx = 0;
	while (word_list_box->GetCount() < MAX_DISPLAY_NUM && 
		   word_idx < word_list.size()) {

		word_t word_in_list = word_list[word_idx];
		word_list_box->Append(wxString(word_in_list.word) + ": " + word_in_list.definition);
		word_idx++;
	}

	on_first_page = true;
	if (word_idx < MAX_DISPLAY_NUM) {
		on_last_page = true;
	}
	else {
		next_page->SetLabel("Next Page");
		next_page->Bind(wxEVT_BUTTON, &DictView::NextPage, this);
		on_last_page = false;
	}
}

void DictView::NextPage(wxCommandEvent& event) {

	word_list_box->Clear();
	while (word_list_box->GetCount() < MAX_DISPLAY_NUM &&
		word_idx < word_list.size()) {

		word_t word_in_list = word_list[word_idx];
		word_list_box->Append(wxString(word_in_list.word) + ": " + word_in_list.definition);
		word_idx++;
	}
	if (word_idx == word_list.size() - 1) {
		on_last_page = true;
		next_page->Unbind(wxEVT_BUTTON, &DictView::NextPage, this);
		next_page->SetLabel("");
	}

	prev_page->Bind(wxEVT_BUTTON, &DictView::PrevPage, this);
	prev_page->SetLabel("Previous");
}

void DictView::PrevPage(wxCommandEvent& event) {

	word_list_box->Clear();
	word_idx -= MAX_DISPLAY_NUM + 1;

	if (word_idx == 0) {
		on_first_page = true;
		prev_page->Unbind(wxEVT_BUTTON, &DictView::PrevPage, this);
		prev_page->SetLabel("");
	}

	while (word_list_box->GetCount() < MAX_DISPLAY_NUM) {
		word_t word_in_list = word_list[word_idx];
		word_list_box->Append(wxString(word_in_list.word) + ": " + word_in_list.definition);
		word_idx++;
	}

	next_page->Bind(wxEVT_BUTTON, &DictView::NextPage, this);
	next_page->SetLabel("Next");
}