#pragma once
#ifndef PRACTICE_MENU
#define PRACTICE_MENU

#include "generic_menu.h"
#include "json_config.h"

#define NUM_OPTIONS (4)
#define MIN_WORDS (4) // due to how helper was made

#define TINY_PANEL_PROPORTION (1)
#define QUESTION_PANEL_PROPORTION (10)
#define BUTTON_PANEL_PROPORTION (1)
#define ANSWER_PANEL_PROPORTION (8)

#define QUESTION_FONT_SCALE (2.25)

#define PRACTICE_BUTTON_WIDTH (100)

class PracticeMenu : public GenericMenuFrame {

	/*
	* Flow of control (using mini menus, if possible):
	* Starting menu -> opens up this frame
	* Language selection -> file selection menu (FOR THE FUTURE)
	* Part of speech selection -> small drop down
	* Open practice menu
	*/

	/*
	* (FOR THE FUTURE:)
	* To record (possibly on file):
	* - Which categories/words are the hardest
	* - Frequency of certain words (could be used to make practice more effective)
	* - "Tag" system: select which words you want to see again
	* - Add in not just Word to Definition, but Definition to Word AND Mixed Mode
	*/

private:
	string curr_dict_path = IGBO_DICT_DIR;
	json curr_dict;
	string curr_category;
	vector <word_t> curr_list;
	word_t current_word;
	int current_word_index; // used for removing from curr_list
	vector <word_t> used_words = {};

	// Going for a kahoot like format here:

	wxSizerFlags default_flags = wxSizerFlags().
		Center().Border(wxALL, 2);

	wxSizerFlags tiny_panel_flags = wxSizerFlags().
		Proportion(TINY_PANEL_PROPORTION).Expand();

	wxSizerFlags question_panel_flags = wxSizerFlags().
		Proportion(QUESTION_PANEL_PROPORTION).Expand();

	wxSizerFlags button_panel_flags = wxSizerFlags().
		Proportion(BUTTON_PANEL_PROPORTION).Expand();

	wxSizerFlags answer_panel_flags = wxSizerFlags().
		Proportion(ANSWER_PANEL_PROPORTION).Expand();

	wxSizerFlags question_box_flags = wxSizerFlags().
		Border(wxALL, 2).Expand();

	wxSizerFlags answer_choice_flags = wxSizerFlags().Expand();

	// Tiny top menu:
	wxPanel* tiny_panel;
	wxBoxSizer* tiny_sizer;
	wxButton* back_to_start;
	wxButton* change_category;

	// Question panel:
	wxPanel* question_panel;
	wxBoxSizer* question_sizer;
	wxStaticText* question_box;

	// Button panel:

	wxPanel* button_panel;
	wxBoxSizer* button_sizer;
	wxButton* start_button;
	wxButton* reshuffle_button;
	wxButton* next_button;

	// Answer panel:
	wxPanel* answer_panel;
	wxGridSizer* answer_sizer;
	wxButton* options[NUM_OPTIONS];

public:
	PracticeMenu(string curr_category);

	void BackToStart(wxCommandEvent& event);
	void ChangeCategory(wxCommandEvent& event);

	void StartReview(wxCommandEvent& event);
	void ReshuffleWords(wxCommandEvent& event);
	void NextWord(wxCommandEvent& event);

	void SubmitChoice(wxCommandEvent& event);

	// Helpers:

	int* GenerateRandomNums(); // finds 4 random indices
	void PresentNewQuestion(); // randomizes the board
};

#endif