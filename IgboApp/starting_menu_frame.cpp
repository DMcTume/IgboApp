#include "starting_menu_frame.h"
#include "vocab_menu_frame.h"
#include "practice_menu.h"

StartingMenu::StartingMenu() : GenericMenuFrame("Starting Menu",
	DEFAULT_WIDTH, DEFAULT_HEIGHT){
	
	getPanel()->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));

	vocab_options = makeMenuButton(getPanel(), getBoxSizer(),
									wxID_ANY, "Edit Vocabulary List",
									default_flags);
	practice_options = makeMenuButton(getPanel(), getBoxSizer(),
									  wxID_ANY, "Enter Practice Mode",
									  default_flags);
	app_description = makeMenuButton(getPanel(), getBoxSizer(),
								 wxID_ANY, "About the App",
								 default_flags);
	
	app_description->Bind(wxEVT_BUTTON,
		&StartingMenu::ShowAppDescription,
		this, wxID_ANY);

	vocab_options->Bind(wxEVT_BUTTON,
		&StartingMenu::OpenVocabFrame,
		this, wxID_ANY);

	practice_options->Bind(wxEVT_BUTTON,
		&StartingMenu::OpenPracticeMenu,
		this, wxID_ANY);
}

void StartingMenu::ShowAppDescription(wxCommandEvent& event) {
	wxLogMessage("Description is yet to be written lol");
}

/*
* Replaces this menu with a new one for editing vocab.
*/
void StartingMenu::OpenVocabFrame(wxCommandEvent& event) {
	int x_coord = 0;
	int y_coord = 0;
	StartingMenu::GetPosition(&x_coord, &y_coord);

	VocabMenu* frame = new VocabMenu();
	frame->SetPosition(wxPoint(x_coord, y_coord));
	frame->Show();
	this->Destroy();
}

/*
* Replaces this menu with a new one for practicing vocab.
* First asks the user which part of speech they want to practice,
* which is then checked by the new frame.
*/
void StartingMenu::OpenPracticeMenu(wxCommandEvent& event) {
	
	// Initial category selection for practice

	wxSingleChoiceDialog* get_category = new wxSingleChoiceDialog(this,
		"Choose a part of speech to practice:", "VOCAB SELECTION",
		{ "NOUNS", "VERBS", "ADJECTIVES", "MISC" });

	if (get_category->ShowModal() == wxID_OK) {
		int x, y;
		this->GetPosition(&x, &y);

		PracticeMenu* frame = new PracticeMenu((string) get_category->GetStringSelection());
		frame->SetPosition(wxPoint(x, y));
		get_category->Destroy();
		this->Destroy();
	}
	else { // Then don't load practice and go back to starting menu
		get_category->Destroy();
	}
	
}