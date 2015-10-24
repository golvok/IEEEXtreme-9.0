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

int main() {
	auto num_cols = get<uint>();
	auto num_rows = get<uint>();
	auto num_chars = get<uint>();

	std::unordered_map<char,std::vector<std::string>> zoomed_in_chars;

	for (uint i = 0; i < num_chars; ++i) {
		std::cin.get();
		char c = std::cin.get();
		std::vector<std::string> zoomed_in_c;
		for (uint row = 0; row < num_rows; ++row) {
			std::cin.get(); // consume newline
			zoomed_in_c.emplace_back();
			for (uint col = 0; col < num_cols; ++col) {
				zoomed_in_c.back().push_back(std::cin.get());
			}
		}
        // std::cout << "read " << c << '\n';
		zoomed_in_chars.emplace(c,std::move(zoomed_in_c));
	}

	auto num_strs = get<uint>();
    // std::cout << num_strs << " 1\n";
    std::cin.get();
    // std::cout << "2\n";
	for (uint i = 0; i < (uint)num_strs; ++i) {
		if (std::cin.eof()) break;
        // std::cout << "3\n";
        std::string str;
        while (true) {
            // std::cout << "4\n";
            char c = std::cin.get();
            // std::cout << c;
            if ( c == '\n' || std::cin.eof()) break;
            str.push_back(c);
        }
        // std::cout << "str=" << str << '\n';
		for (uint row = 0; row < num_rows; ++row) {
			for (char c : str) {
                std::cout << zoomed_in_chars[c][row]; 
			}
			std::cout << '\n';
		}
	}

	return 0;
}
