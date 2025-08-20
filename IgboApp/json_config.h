#pragma once

#ifndef JSON_CONFIG
#define JSON_CONFIG

#include <wx/wx.h> // including after other libraries causes deprecation errors
#include "json.hpp" // includes <string>
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

// Structure for serializing
typedef struct word_structure {
	string word;
	string definition;
	string image;
	string example;
} word_t;

// Actual serialization:
namespace nlohmann {
	template<>
	struct adl_serializer<word_structure> {

		static void to_json(json& j, const word_structure& c) {
			j = json{
				{"word", c.word},
				{"definition", c.definition},
				{"image", c.image},
				{"example", c.example}
			};
		}

		static void from_json(const json& j, word_structure& c) {
			try {
				c.word = j.at("word").get<string>();
				c.definition = j.at("definition").get<string>();
				c.image = j.at("image").get<string>();
				c.example = j.at("example").get<string>();
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

// Makes opening files more convenient:

int open_JSON(fstream* file, string path, char mode);

// Makes creating backups easier:
/*
* This function must be run BEFORE opening original file
*/

int create_backup(fstream *original, fstream *backup,
				  string original_path, string backup_path);

#endif
