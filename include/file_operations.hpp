#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

namespace games {

    namespace detail {

        bool openFile(auto const& file_name, auto& file_stream) {
            file_stream.open(file_name);
            file_stream.imbue(std::locale(file_stream.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
            if (!file_stream.is_open()) {
                std::cerr << "Error: Unable to open file_stream.\n";
                return false;
            }
            return true;
        }

        bool processLine(auto const& line, auto& word_def_map, size_t index) {
            std::wistringstream iss(line);
            std::wstring target_word, english_definition;

            if (iss >> target_word) {
                std::getline(iss, english_definition);
                std::for_each(target_word.begin(), target_word.end(), [](auto& c) {
                    c = std::tolower(c, std::locale());
                    });

                word_def_map.at(index) = std::make_pair(target_word, english_definition);
                return true;
            }
            return false;
        }

    }

    bool loadVocabularyFromFile(auto const& file_name, auto& word_def_map) {
        std::wifstream file_stream;
        if (!detail::openFile(file_name, file_stream))
            return false;

        size_t index = 0;
        std::wstring line;
        while (std::getline(file_stream, line)) {
            if (!detail::processLine(line, word_def_map, index++)) {
                std::cerr << "Error: Unable to process line.\n";
                return false;
            }
        }

        file_stream.close();
        return true;
    }

}