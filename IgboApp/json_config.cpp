#include "json_config.h"
#include <iostream>

/// For wstring to utf8 conversion
#include <codecvt>
#include <locale>

/*
* Opens the file at the path with the given mode and 
* updates the given file object.
* Returns non-zero upon encountering an error.
*/
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
			return OPEN_ERROR;
		}
	}

	catch (const exception& e) {
		return OPEN_ERROR;
	}

	if (file->fail()) {
		file->close();
		return OPEN_ERROR;
	}

	if (!file->is_open()) {
		return OPEN_ERROR;
	}

	return OPEN_SUCCESS;
}

/*
* Creates a backup for the original file using both original's and backup's paths,
* updating their file objects accordingly.
* Reminder that this method should be called BEFORE attempting to write to the original
* (because when the original is opened for writing, all the data will be lost).
* Returns non-zero upon encountering an error.
*/
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

string wstring_to_utf8(const wstring& wide_string) {
	static wstring_convert<codecvt_utf8_utf16<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(wide_string);
}

wstring utf8_to_wstring(const string& utf8_string) {
	static wstring_convert< codecvt_utf8_utf16<wchar_t>> utf8_conv;
	return utf8_conv.from_bytes(utf8_string);
}