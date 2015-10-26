#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <unordered_map>


template<typename T, typename STREAM>
T get(STREAM& is) {
	T t;
	is >> t;
	return t;
}

template<typename T>
T get() {
	return get<T>(std::cin);
}

struct print_printable { };

template<typename T, typename STREAM>
auto operator<<(STREAM& os, const T& t) -> decltype(static_cast<const print_printable*>(&t),os) {
	t.print(os);
	return os;
}

using LetterArray = std::array<uint,26>;

template<typename STREAM>
auto operator<<(STREAM& os, const LetterArray& arr) -> decltype(os) {
	for (uint i = 0; i < 26; ++i) {
		std::cout << (char)('a' + i) << ':' << arr[i] << ' ';
	}
	return os;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	auto num_test_cases = get<uint>();

	for (uint test_case_num = 0; test_case_num < num_test_cases; ++test_case_num) {
		auto num_words = get<uint>();
		auto num_dict_strings = get<uint>();

		LetterArray max_num_letters_seen;
		std::fill(max_num_letters_seen.begin(),max_num_letters_seen.end(),0);

		std::cin.get(); // consume newline
		LetterArray num_letters_seen_in_this_word;
		for (uint word_num = 0; word_num < num_words; ++word_num) {
			std::fill(num_letters_seen_in_this_word.begin(),num_letters_seen_in_this_word.end(),0);
			uint len = 0;
			while (true) {
				char c = std::cin.get();
				if (c == '\n') {
					break;
				}
				len += 1;
				std::cout << c;
				num_letters_seen_in_this_word[c - 'a'] += 1;
			}

			std::cout << std::string(20 - len, ' ') << " : " << num_letters_seen_in_this_word << '\n';

			for (uint i = 0; i < 26; ++i) {
				if (num_letters_seen_in_this_word[i] > max_num_letters_seen[i]) {
					max_num_letters_seen[i] = num_letters_seen_in_this_word[i];
				}
			}
		}

		std::cout << "MAXIMUM              : " << max_num_letters_seen << '\n' << '\n';

		LetterArray num_letters_in_dict_string;
		for (uint string_num = 0; string_num < num_dict_strings; ++string_num) {
			std::fill(num_letters_in_dict_string.begin(),num_letters_in_dict_string.end(),0);
			uint len = 0;
			while (true) {
				char c = std::cin.get();
				if (c == '\n') {
					break;
				}
				len += 1;
				std::cout << c;
				num_letters_in_dict_string[c - 'a'] += 1;
			}

			std::cout << std::string(20 - len, ' ') << " : " << num_letters_seen_in_this_word << '\n';

			uint num_letters_missing = 0;
			uint num_letters_too_many = 0;
			for (uint i = 0; i < 26; ++i) {
				if (num_letters_in_dict_string[i] < max_num_letters_seen[i]) {
					num_letters_missing += max_num_letters_seen[i] - num_letters_in_dict_string[i];
					std::cout << "missing " << max_num_letters_seen[i] - num_letters_in_dict_string[i] << ' ' << (char)('a' + i) << '\n';
				}
				if (num_letters_in_dict_string[i] > max_num_letters_seen[i]) {
					num_letters_too_many += num_letters_in_dict_string[i] - max_num_letters_seen[i];
					std::cout << "too many by " << num_letters_in_dict_string[i] - max_num_letters_seen[i] << ' ' << (char)('a' + i) << '\n';
				}
			}

			std::cout << "ANSWER: ";
			if (num_letters_missing > 0) {
				std::cout << "No " << num_letters_missing;
			} else {
				std::cout << "Yes ";
				if (num_letters_too_many == 0) {
					std::cout << "Yes";
				} else {
					std::cout << "No";
				}
			}
			std::cout << '\n';
		}
	}
}