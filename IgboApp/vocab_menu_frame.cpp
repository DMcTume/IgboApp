#include "vocab_menu_frame.h"
#include "starting_menu_frame.h"
#include "content_menu.h"
#include "json_config.h" // json.hpp has no guards

VocabMenu::VocabMenu() : GenericMenuFrame("Vocab Menu",
	DEFAULT_WIDTH, DEFAULT_HEIGHT) {

	getPanel()->SetBackgroundColour(wxTheColourDatabase->Find("TAN"));

	wxButton** content_buttons[NUM_CONTENT_BUTTONS] = 
			{&noun_button, &verb_button, &descriptor_button, &misc_button };

	const char* button_names[NUM_CONTENT_BUTTONS] =
			{"NOUNS", "VERBS", "ADJECTIVES", "MISC"};

	for (int i = 0; i < NUM_CONTENT_BUTTONS; i++) {
		wxButton** button = content_buttons[i];
		(*button) = makeMenuButton(getPanel(), getBoxSizer(), wxID_ANY,
			button_names[i], default_flags);
		(*button)->Bind(wxEVT_BUTTON, &VocabMenu::OpenContent, this, wxID_ANY);
	}

	back_button = makeMenuButton(getPanel(), getBoxSizer(),
		wxID_ANY, "BACK TO STARTING MENU",
		default_flags);

	back_button->Bind(wxEVT_BUTTON, &VocabMenu::BackToStart,
		this, wxID_ANY);
}

/*
* Replaces the current frame with the StartingMenu
*/
void VocabMenu::BackToStart(wxCommandEvent& event) {
	int x = 0;
	int y = 0;
	this->GetPosition(&x, &y);

	StartingMenu* frame = new StartingMenu();
	frame->SetPosition(wxPoint(x, y));
	frame->Show();

	this->Destroy();
}

/*
* Opens up the ContentMenu for the selected part of speech, 
* thus allowing the user to edit the dictionary.
* First attempts to load in the dictionary's json;
* if loading throws, the ContentMenu is not opened and a 
* message is presented.
*/
void VocabMenu::OpenContent(wxCommandEvent& event) {
	
	fstream dict_file;
	json dict;
	try {

		if (open_JSON(&dict_file, IGBO_DICT_DIR, 'R')) {
			throw new runtime_error("Could not open json file!");
		}

		dict = json::parse(dict_file);
		dict_file.close();
	}
	catch (const exception& e) {
		wxLogMessage(e.what());

		if (dict_file) {
			dict_file.close();
		}
	}
	
	int x, y = 0;
	this->GetPosition(&x, &y);

	wxButton* button_pressed = (wxButton*)event.GetEventObject();
	string category = (string) button_pressed->GetLabel().ToAscii();

	ContentMenu* frame = new ContentMenu((category + " LIST").c_str(), category, dict);
	frame->SetPosition(wxPoint(x, y));
	frame->Show();
	this->Destroy();
}
