#pragma once

#ifndef DICT_VIEW_H
#define DICT_VIEW_H

#include "generic_menu.h"
#include "json_config.h"

#define WINDOW_WIDTH (300)
#define WINDOW_HEIGHT (350)

#define LIST_WIDTH (200)
#define LIST_HEIGHT (300)

#define MAX_DISPLAY_NUM (3)

class DictView : public wxDialog {

private:

	wxSizerFlags default_flags = wxSizerFlags().
		Center().Border(wxALL, 2);

	wxPanel* main_panel;
	wxBoxSizer* main_sizer;
	
	vector<word_t> word_list;
	int word_idx;
	wxListBox* word_list_box;

	wxBoxSizer* button_sizer;
	wxButton* prev_page;
	wxButton* next_page;
	bool on_first_page;
	bool on_last_page;
	
	void PrevPage(wxCommandEvent& event);
	void NextPage(wxCommandEvent& event);

public:
	DictView(const char* menu_name, vector<word_t> word_list);
};

#endif