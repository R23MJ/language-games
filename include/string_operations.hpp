#pragma once
#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>

namespace games {

    namespace detail {

        using CharFrequencyMap = std::unordered_map<wchar_t, int>;

        void fillFromString(CharFrequencyMap& frequencyMap, std::wstring_view letters) {
            std::ranges::for_each(letters, [&frequencyMap](wchar_t c) {
                ++frequencyMap[c];
                });
        }

    }

    bool canSpell(std::wstring_view letters, std::wstring_view word) {
        detail::CharFrequencyMap frequencyMap;
        detail::fillFromString(frequencyMap, letters);

        return std::ranges::all_of(word, [&frequencyMap](auto const& c) -> bool {
            auto it = frequencyMap.find(c);
            if (it != frequencyMap.end() && it->second > 0) {
                --it->second;
                return true;
            }
            return false;
            });
    }

}