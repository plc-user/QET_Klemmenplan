/*
 * Copyright (c) 2025 plc-user
 *
 * It is NOT allowed to use this software or any of the
 * associated files as sample data for the purposes of
 * building machine learning models.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "helpers.h"

#include <algorithm>
#include <regex>        // vergrößert binary um ca. 140 kB


// Reihen-Numerierung mit Buchstaben-Kombination:
std::string NumberToLetters(uint64_t number) {
    if (number <= 0) {
        return "";
    }
    std::string result = "";
    while (number > 0) {
        number--;
        result += (char)('A' + number % 26);
        number /= 26;
    }
    std::reverse(result.begin(), result.end());
    return result;
}


// gebe String vom Anfang bis zum Trennzeichen zurück
std::string LeftOfInterpunct(std::string& s) {
    if (s.size() == 0) return "";
    const size_t i = s.find_first_of(".:;");
    return s.substr(0, i);
}

// gebe String vom Trennzeichen bis zum Ende zurück
std::string RightOfInterpunct(std::string& s) {
    if (s.size() == 0) return "";
    const size_t i = s.find_first_of(".:;");
    return ( i+1 == std::string::npos ) ? "" : s.substr(i+1);
}

// führenden Whitespace entfernen
std::string left_trim(const std::string& s) {
    const size_t i = s.find_first_not_of(" \n\r\t\f\v");
    return ( i == std::string::npos ) ? "" : s.substr(i);
}

//
// Source:
// https://rosettacode.org/wiki/Natural_sorting#C++
//
// Pad each numeric character with leading zeros to a total length of 20
std::string zero_padding(std::string s) {
    std::string text = RightOfInterpunct(s); // eingefügt, weil in der Lambda-Funktion keine "Verkettung" möglich ist.

	const static std::regex digits("-?\\d+");
	const std::vector<std::string> matches{
		std::sregex_token_iterator{text.begin(), text.end(), digits, 0}, std::sregex_token_iterator{} };

	std::vector<uint64_t> indexes = { };
	for ( const std::string& match : matches ) {
		indexes.emplace_back(text.find(match));
	}

	uint32_t extra_index = 0;
	for ( uint64_t i = 0; i < matches.size(); ++i ) {
		uint64_t start_pos = text.find(matches[i], indexes[i] + extra_index);
		text = text.substr(0, start_pos) + std::string(20 - matches[i].size(), '0') + text.substr(start_pos);
		extra_index += 20 - matches[i].size();
	}
	return text;
}
//
// END - Source:
// https://rosettacode.org/wiki/Natural_sorting#C++
//

