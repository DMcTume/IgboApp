/*
* This frame allows the user to practice matching words with their definitions
* (and will include other forms of matching in the future).
* The design is very similar to Kahoot's interface.
*/

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

	// Returns to the StartingMenu
	void BackToStart(wxCommandEvent& event);

	// Changes the current part of speech
	void ChangeCategory(wxCommandEvent& event);

	// Initiates review (starts a new streak)
	void StartReview(wxCommandEvent& event);

	// Resets the review (ends the current streak so the user can start a new one)
	void ReshuffleWords(wxCommandEvent& event);

	// Moves on the next word after the answer is revealed
	void NextWord(wxCommandEvent& event);

	// Submits the clicked option as the user's answer
	void SubmitChoice(wxCommandEvent& event);

	// Helpers:

	// Finds four random indices from the current list of words
	int* GenerateRandomIndices(); 

	// "Randomizes" the board; for each new question
	void PresentNewQuestion(); 

	// Restarts the practice session
	void RestartPractice();
};

#endif