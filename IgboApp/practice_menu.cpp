#include "practice_menu.h"
#include "starting_menu_frame.h"
#include <vector>

// For random numbers
#include <ctime>
#include <cstdlib>

/*
* Firstly, the frame attempts to load in the dictionary's json. If it cannot be loaded
* in, the user is left on the StartingMenu.
* 
* Additionally, the frame checks if the chosen category from the StartingMenu
* has at least 4 words (because there must be at least four options available).
* If the category is not big enough, then the frame will not successfully open and the
* user will left on the StartingMenu.
*/
PracticeMenu::PracticeMenu(string curr_category) : 
	GenericMenuFrame("Practice Menu", 845, 520) {

	// Get json:
	fstream dict_file;
	try {
		if (open_JSON(&dict_file, curr_dict_path, 'R')) {
			throw new runtime_error("Failed to open file for reading!");
		}
		curr_dict = json::parse(dict_file);
		dict_file.close();
	}
	catch (const exception& e) {
		wxLogMessage("Sorry, could not load practice!\n" + (string) e.what());
		if (dict_file) {
			dict_file.close();
		}
		
		int x, y;
		this->GetPosition(&x, &y);

		StartingMenu* frame = new StartingMenu();
		frame->SetPosition(wxPoint(x, y));
		frame->Show();
		this->Destroy();
	}

	// Check category:
	vector<word_t> temp_list = curr_dict.at(curr_category);
	
	if (temp_list.size() < MIN_WORDS) {
		wxLogMessage("Sorry, the selected category needs at least 4 words to practice with!\n"
			"This can be changed in the Vocab Menu's edit features");
		int x, y;
		this->GetPosition(&x, &y);

		StartingMenu* frame = new StartingMenu();
		frame->SetPosition(wxPoint(x, y));
		frame->Show();
		this->Destroy();
	}
	else {
		this->curr_list = temp_list;
		this->Show();
	}

	this->curr_category = curr_category;
	this->getPanel()->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));

	// TOP TINY PANEL:
	tiny_panel = new wxPanel(this->getPanel(), wxID_ANY, wxDefaultPosition,
		wxDefaultSize);
	tiny_sizer = new wxBoxSizer(wxHORIZONTAL);
	tiny_panel->SetSizer(tiny_sizer);
	tiny_sizer->SetSizeHints(this->getPanel());

	// QUESTION PANEL:
	question_panel = new wxPanel(this->getPanel(), wxID_ANY, wxDefaultPosition,
		wxDefaultSize);
	question_sizer = new wxBoxSizer(wxHORIZONTAL);
	question_panel->SetSizer(question_sizer);
	question_sizer->SetSizeHints(this->getPanel());

	// BUTTON PANEL:
	button_panel = new wxPanel(this->getPanel());
	button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_panel->SetSizer(button_sizer);
	button_sizer->SetSizeHints(this->getPanel());

	// ANSWER PANEL:
	answer_panel = new wxPanel(this->getPanel(), wxID_ANY, wxDefaultPosition,
		wxDefaultSize);
	answer_sizer = new wxGridSizer(2, 2, wxSize(0, 0));
	answer_panel->SetSizer(answer_sizer);
	answer_sizer->SetSizeHints(this->getPanel());
	
	// ADDING MAIN SIZERS:

	this->getBoxSizer()->Add(tiny_panel, tiny_panel_flags);
	this->getBoxSizer()->Add(question_panel, question_panel_flags);
	this->getBoxSizer()->Add(button_panel, button_panel_flags);
	this->getBoxSizer()->Add(answer_panel, answer_panel_flags);

	// TOP TINY PANEL CONTROLS:

	back_to_start = new wxButton(tiny_panel, wxID_ANY, "Back to Starting Menu");
	tiny_sizer->Add(back_to_start, default_flags);
	tiny_sizer->AddStretchSpacer(8); // other buttons have prop = 1 by default
	change_category = new wxButton(tiny_panel, wxID_ANY, "Change Part of Speech");
	tiny_sizer->Add(change_category, default_flags);

	back_to_start->Bind(wxEVT_BUTTON, &PracticeMenu::BackToStart, this);
	change_category->Bind(wxEVT_BUTTON, &PracticeMenu::ChangeCategory, this);

	// QUESTION PANEL CONTROLS:

	question_box = new wxStaticText(question_panel, wxID_ANY, "Press the small button below to start!",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);
	question_box->SetFont(question_box->GetFont().Scale(QUESTION_FONT_SCALE));
	
	question_sizer->AddStretchSpacer();
	question_sizer->Add(question_box, question_box_flags);
	question_sizer->AddStretchSpacer();

	// BUTTON PANEL CONTROLS:

	start_button = new wxButton(button_panel, wxID_ANY, "Start", wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	reshuffle_button = new wxButton(button_panel, wxID_ANY, "", wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	next_button = new wxButton(button_panel, wxID_ANY, "", wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));

	button_sizer->AddStretchSpacer();
	button_sizer->Add(start_button, default_flags);
	button_sizer->Add(reshuffle_button, default_flags);
	button_sizer->Add(next_button, default_flags);
	button_sizer->AddStretchSpacer();

	start_button->Bind(wxEVT_BUTTON, &PracticeMenu::StartReview, this);

	// ANSWER PANEL CONTROLS: 

	for (int i = 0; i < NUM_OPTIONS; i++) {
		wxButton** button = &(options[i]);
		*button = new wxButton(answer_panel, wxID_ANY, "");
		answer_sizer->Add(*button, answer_choice_flags);
	}

	this->Layout(); // needed to update after this frame shows itself
}

// Helpers:

/*
* This function finds four indices from the curr_list that have not been used
* (indices whose words are not already in used_words).
* It returns an allocated array of the indices that are found.
*/
int* PracticeMenu::GenerateRandomIndices() {
	srand(time(NULL));

	int* random_nums = new int[NUM_OPTIONS] {-1, -1, -1, -1};
	if (!random_nums) {
		wxLogMessage("Failed to randomize word selection! (allocation error)");
	}

	// Find FOUR indices randomly
	// Do not use indices that have already been used

	int curr_index = 0;
	int word_list_len = curr_list.size();

	while (curr_index < NUM_OPTIONS) {
		int rand_num = rand() % word_list_len;

		bool in_list_already = false;

		for (int i = 0; i < NUM_OPTIONS; i++) {
			if (*(random_nums + i) == rand_num) {
				in_list_already = true;
			}
		}

		if (in_list_already) { // try again
			continue;
		}

		else { // add it in
			*(random_nums + curr_index) = rand_num;
			curr_index++;
		}
	}

	return random_nums;
}

/*
* This function "randomizes" the board by randomly choosing one of four words
* chosen by GenerateRandomIndices. 
* It then fills the answer board (randomly) with four possible answers (only one
* of which is correct, of course).
*/
void PracticeMenu::PresentNewQuestion() {
	int* rand_nums = this->GenerateRandomIndices();
	srand(time(NULL));
	int chosen_word_index = rand() % NUM_OPTIONS;

	this->current_word = curr_list.at(rand_nums[chosen_word_index]);
	this->current_word_index = rand_nums[chosen_word_index];
	
	question_box->SetLabel(wxString("What is \"") + current_word.word + 
		wxString("\" in English?"));

	for (int i = 0; i < NUM_OPTIONS; i++) {
		wxButton* button = options[i];
		int index = rand_nums[i];
		button->SetLabel(curr_list.at(index).definition);
	}

	delete(rand_nums);

	for (wxButton* button : options) {
		button->Bind(wxEVT_BUTTON, &PracticeMenu::SubmitChoice, this);
	}
}

/*
* Restarts the practice session.
* Used in Reshuffle and ChangeCategory
*/
void PracticeMenu::RestartPractice() {

	vector <word_t> temp = curr_dict.at(curr_category);
	curr_list = temp;
	used_words = {};

	start_button->SetLabel("Start");
	start_button->Bind(wxEVT_BUTTON, &PracticeMenu::StartReview, this);

	reshuffle_button->SetLabel("");
	reshuffle_button->Unbind(wxEVT_BUTTON, &PracticeMenu::ReshuffleWords, this);

	next_button->SetLabel("");
	// If needed:
	try {
		next_button->Unbind(wxEVT_BUTTON, &PracticeMenu::NextWord, this);
	}
	catch (const exception) {};

	for (wxButton* button : options) {
		button->SetBackgroundColour(wxTheColourDatabase->Find("WHITE"));
		button->SetLabel("");
	}
	question_box->SetLabel("Press the small button below to start!");
}

// Tiny panel methods:

/*
* Returns sthe user to the starting menu,
* upon additional affirmation.
*/
void PracticeMenu::BackToStart(wxCommandEvent& event) {
	wxMessageDialog* exit_check = new wxMessageDialog(this, "Are you sure you want to exit practice?",
		"EGRESS INITIATED", wxYES_NO);

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

/*
* Changes the current part of speech.
* If the new category selected has fewer than 4 words, 
* then it will fail to load and the dialog will close.
*/

void PracticeMenu::ChangeCategory(wxCommandEvent& event) {

	wxSingleChoiceDialog* get_category = new wxSingleChoiceDialog(this,
		"Choose a part of speech to practice:", "VOCAB SELECTION",
		{ "NOUNS", "VERBS", "ADJECTIVES", "MISC" });

	if (get_category->ShowModal() == wxID_OK) { // check the category as well

		string temp_category = (string) get_category->GetStringSelection();
		vector <word_t> temp_list = curr_dict.at(temp_category);

		if (temp_category == curr_category) {
			wxLogMessage("Category is already in play, try again!");
		}
		else if (temp_list.size() < MIN_WORDS) {
			wxLogMessage("Sorry, the selected category needs at least 4 words to practice with!\n"
				"This can be changed in the Vocab Menu's edit features");
		}
		else {
			this->curr_category = temp_category;
			PracticeMenu::RestartPractice();
		}

		get_category->Destroy();
	}
	else {
		get_category->Destroy();
	}
}

// Button panel methods:

/*
* Starts the review by presenting a new question,
* disabling the start button, and presenting the reshuffle button.
*/
void PracticeMenu::StartReview(wxCommandEvent& event) {

	this->PresentNewQuestion();

	// Blank the start button:
	start_button->SetLabel("");
	start_button->Unbind(wxEVT_BUTTON, &PracticeMenu::StartReview, this);

	reshuffle_button->SetLabel("Reshuffle Words");
	reshuffle_button->Bind(wxEVT_BUTTON, &PracticeMenu::ReshuffleWords, this);
}

/*
* Presents a new question after answering an old question.
* Because words may be marked as used and removed from the current
* list of available words, if the amount of words available becomes less than
* 4, than the user will prompted to reshuffle or choose another category.
*/
void PracticeMenu::NextWord(wxCommandEvent& event) {
	
	for (wxButton* button : options) {
		button->SetBackgroundColour(wxTheColourDatabase->Find("WHITE"));
		button->SetLabel("");
	}
	
	next_button->SetLabel("");
	next_button->Unbind(wxEVT_BUTTON, &PracticeMenu::NextWord, this);

	// Check curr_list if another round can be done:

	if (curr_list.size() < NUM_OPTIONS) {
		wxLogMessage("Ran out of new words to practice with!\n"
					"Reshuffle or choose another category to practice some more...");
	}
	else {
		this->PresentNewQuestion();
	}
}

/*
* Restarts the user's current streak.
* In other words, makes all used words available again and allows the user
* to restart practice.
*/
void PracticeMenu::ReshuffleWords(wxCommandEvent& event) {

	wxMessageDialog* check = new wxMessageDialog(this, "Are you sure you want to reshuffle?\n"
		"It will reset your current streak!", "RESTART PROMPTED", wxYES_NO);

	if (check->ShowModal() == wxID_YES) {
		PracticeMenu::RestartPractice();
	}
	check->Destroy();

	for (wxButton* button : options) {
		button->SetBackgroundColour(wxTheColourDatabase->Find("WHITE"));
		button->SetLabel("");
	}	
}

// Answer panel methods:

/*
* Clicking an answer buttons submits its content as an answer.
* Furthermore, if the answer was correct, than that word won't show up again in practice
* (it's marked as "used").
*/
void PracticeMenu::SubmitChoice(wxCommandEvent& event) {
	wxString submission = ((wxButton*)event.GetEventObject())->GetLabel();
	
	if (submission == current_word.definition) {
		question_box->SetLabel(question_box->GetLabel() + "\n Correct!");
		
		// Move past word into used_words, remove from curr_list
		used_words.push_back(current_word);
		curr_list.erase(curr_list.begin() + current_word_index);
	}
	else {
		question_box->SetLabel(question_box->GetLabel() + "\n Incorrect! \n" 
			"Correct Answer: " + current_word.definition);
	}

	for (wxButton* button : options) {
		if (button->GetLabel() == current_word.definition) {
			button->SetBackgroundColour(wxTheColourDatabase->Find("GREEN"));
		}
		else {
			button->SetBackgroundColour(wxTheColourDatabase->Find("RED"));
		}
		button->Unbind(wxEVT_BUTTON, &PracticeMenu::SubmitChoice, this);
		next_button->SetLabel("Next Word");
		next_button->Bind(wxEVT_BUTTON, &PracticeMenu::NextWord, this);
	}
}