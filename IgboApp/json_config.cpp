#include "json_config.h"
#include <iostream>

// LogMessages exist for debugging purposes

int open_JSON(fstream* file, string path, char mode) {

	if (file->is_open()) {
		file->close();
	}

	try {

		if (mode == 'R') {
			file->open(path, ios::in);
		}
		else if (mode == 'W') {
			file->open(path, ios::out);
		}
		else {
			// wxLogMessage("From open_file(): open mode is invalid");
			return OPEN_ERROR;
		}
	}

	catch (const exception& e) {
		/*stringstream ss;
		ss << "From open_file() of: " << path << " " << e.what();
		string error_msg = ss.str();

		wxLogMessage(error_msg)*/
		return OPEN_ERROR;
	}

	if (file->fail()) {
		//wxLogMessage("From open_file(): " + path + " opened in poor state");
		file->close();
		return OPEN_ERROR;
	}

	if (!file->is_open()) {
		// wxLogMessage("From open_file(): could not open " + path);
		return OPEN_ERROR;
	}

	return OPEN_SUCCESS;
}

int create_backup(fstream* original, fstream* backup,
				  string original_path, string backup_path) {
	try {

		if (open_JSON(original, original_path, 'R')) {
			throw new runtime_error("Opening original for copying failed");
		}

		json ORIGINAL_DICT{ json::parse(*original) };

		if (open_JSON(backup, backup_path, 'W')) {
			original->close();
			throw new runtime_error("Opening backup for writing failed");
		}

		*backup << ORIGINAL_DICT.dump(2);

		original->close();
		// backup->close(); keep open for potential writing errors
	}
	catch (const exception& e) {
		stringstream ss;
		ss << "ABORTING: create_backup() failed: " << e.what();
		wxLogMessage(ss.str());

		backup->close();
		original->close();
		return BACKUP_FAILURE;
	}

	return BACKUP_SUCCESS;
}