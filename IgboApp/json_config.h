/*
* This file includes libraries and features
* necessary for working with the dictionary's json, specifically
* serialization for word structures, file handling methods, and 
* paths for files (that last one may change in the future).
* Note that whenever json.hpp is needed (such as for json objects), 
* this libaray is included instead because json.hpp lacks header guards.
*/

#pragma once

#ifndef JSON_CONFIG
#define JSON_CONFIG

#include <wx/wx.h> // including after other libraries causes deprecation errors
#include "json.hpp"
#include <fstream>
#include <sstream>

#define OPEN_SUCCESS (0)
#define OPEN_ERROR (1)

#define BACKUP_SUCCESS (0)
#define BACKUP_FAILURE (1)

#define IGBO_DICT_DIR ("dict_folder/igbo_dictionary.json")
#define IGBO_BACKUP_DIR ("dict_folder/igbo_backup.json")

using json = nlohmann::json;
using namespace std;

// Structure for serialization
typedef struct word_structure {
	wstring word;
	wstring definition;
	string image;  // will likely be a path, so no need for wstring
  	wstring example;
} word_t;

// Sources for conversion: DeepSeek and https://json.nlohmann.me/home/faq/#wide-string-handling
/*
* The following two functions are for converting between the 
* system's default encoding space (e.g. likely UTF-16 for me, because I use Windows)
* and JSON's UTF-8 string encoding space.
* These two functions are what allow Unicode to be readable both
* in program and in JSON.
*/

string wstring_to_utf8(const wstring& wide_string);

wstring utf8_to_wstring(const string& utf8_string);

// Main source that helped: https://www.youtube.com/watch?app=desktop&v=Sa8bdVogGIo&t=476s
// Actual serialization:
namespace nlohmann {
	template<>
	struct adl_serializer<word_structure> {

		static void to_json(json& j, const word_structure& c) {
			j = json{
				{"word", wstring_to_utf8(c.word)},
				{"definition", wstring_to_utf8(c.definition)},
				{"image", c.image},
				{"example", wstring_to_utf8(c.example)}
			};
		}

		static void from_json(const json& j, word_structure& c) {
			try {
				c.word = utf8_to_wstring(j.at("word").get<string>());
				c.definition = utf8_to_wstring(j.at("definition").get<string>());
				c.image = j.at("image").get<string>();
				c.example = utf8_to_wstring(j.at("example").get<string>());
			}
			catch (const exception& e) {
				stringstream error_stream;
				error_stream << "JSON Format Error: " << e.what();
				string error_msg = error_stream.str();

				wxLogMessage(error_msg);
			}
		}
	};
}

/*
 * Opens the json file for whatever mode is indicated.
 */
int open_JSON(fstream* file, string path, char mode);

/*
* Facilitates creating backups for files.
*/
int create_backup(fstream *original, fstream *backup,
				  string original_path, string backup_path);

#endif
