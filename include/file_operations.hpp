#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>

namespace games {

    namespace detail {

        bool openFile(auto const& file_name, auto& file) {
            file.open(file_name);
            file.imbue(std::locale(file.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
            if (!file.is_open()) {
                std::cerr << "Error: Unable to open file.\n";
                return false;
            }
            return true;
        }

        bool processLine(auto const& line, auto& word_def_map) {
            std::wistringstream iss(line);
            std::wstring target_word, english_definition;

            if (iss >> target_word) {
                std::getline(iss, english_definition);
                std::for_each(target_word.begin(), target_word.end(), [](auto& c) {
                    c = std::tolower(c, std::locale());
                    });

                word_def_map.push_back({ target_word, english_definition });
                return true;
            }
            return false;
        }

    }

    bool loadVocabularyFromFile(auto const& file_name, auto& word_def_map) {
        std::wifstream file;
        if (!detail::openFile(file_name, file)) {
            return false;
        }

        std::wstring line;
        while (std::getline(file, line)) {
            if (!detail::processLine(line, word_def_map)) {
                std::cerr << "Error: Unable to process line.\n";
                return false;
            }
        }

        file.close();
        return true;
    }

}