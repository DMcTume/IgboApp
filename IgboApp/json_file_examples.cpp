/*

Needed includes:

#include "json_config.h"
#include <cstdio>


void VocabMenu::TestREAD(wxCommandEvent& event) {

	fstream file;
	if (open_JSON(&file, IGBO_DICT_DIR, 'R')) {
		return; // error occured
	}

	// Remember to cover potential json errors
	try {
		word_structure some_word = json::parse(file).at("NOUNS")[0];
		wxLogMessage(some_word.word + ": " + some_word.definition);
	}
	catch (const exception &e) {
		stringstream ss;
		ss << "Could not read JSON: " << e.what();
		wxLogMessage(ss.str());
		file.close();
	}

	file.close();
}

void VocabMenu::TestWRITE(wxCommandEvent& event) {

	// Probably better to move these openings into try-catch

	fstream igbo_dict;
	fstream igbo_backup;

	if (create_backup(&igbo_dict, &igbo_backup, IGBO_DICT_DIR, IGBO_BACKUP_DIR)) {
		igbo_backup.close();
		return;
	}

	// Now actually write data you want
	try {

		if (open_JSON(&igbo_dict, IGBO_DICT_DIR, 'R')) {
			igbo_backup.close();
			wxLogMessage("ABORTING TestWrite(): could not copy dict for writing");
			return;
		}

		json IGBO_DICT{ json::parse(igbo_dict) };

		if (open_JSON(&igbo_dict, IGBO_DICT_DIR, 'W')) {
			throw new runtime_error("could not open for writing properly");
		}

		IGBO_DICT.at("NOUNS")[1] = word_structure{"2nd word", "2nd def", "2nd img", "2nd ex"};
		igbo_dict << IGBO_DICT.dump(2);

		igbo_backup.close();
		remove(IGBO_BACKUP_DIR);  // requires <cstdio>

		igbo_dict.close();
	}
	catch (const exception& e) {
		remove(IGBO_DICT_DIR);
		rename(IGBO_BACKUP_DIR, IGBO_DICT_DIR);
		igbo_backup.close();
		igbo_dict.close();

		stringstream ss;
		ss << "ABORTING TestWrite(): " << e.what();
		wxLogMessage(ss.str());
	}

}


*/