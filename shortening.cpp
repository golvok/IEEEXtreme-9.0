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

std::string base62Encode(uint64_t num) {
	std::string result;

	while (num != 0) {
		uint digit_value = num % 62;
		num /= 62;

		if (digit_value < 10) {
			result += (char)('0' + digit_value);
		} else if (digit_value < 36) {
			result += (char)('a' + (digit_value-10));
		} else {
			result += (char)('A' + (digit_value-36));
		}
	}

	std::reverse(result.begin(),result.end());

	return result;
}

const size_t XOR_CHUNK = 8;

int main() {
	const auto company_url = get<std::string>();
	const auto num_urls = get<uint>();

	std::vector<std::pair<std::string,size_t>> strings_and_lengths;
	for (uint i = 0; i < num_urls; ++i) {
		const auto url = get<std::string>();
		strings_and_lengths.emplace_back(url.substr(url.size()-XOR_CHUNK),url.size());
	}

	for (const auto& str_and_len : strings_and_lengths) {
		const size_t start = (str_and_len.second - XOR_CHUNK) % company_url.size();
		std::array<char,XOR_CHUNK> xor_results;
		for (size_t i = 0; i < xor_results.size(); ++i) {
			xor_results[i] = str_and_len.first[i] ^ company_url[(start + i) % company_url.size()];
		}

		// for (const auto& num : xor_results) {
		// 	std::cout << (int)(num) << ' ';
		// }
		// std::cout << '\n';

		uint64_t to_be_62_encoded;
		for (const auto& num : xor_results) {
			to_be_62_encoded = to_be_62_encoded << 8;
			to_be_62_encoded += num;
		}

		// std::cout << to_be_62_encoded << '\n';

		std::string base62 = base62Encode(to_be_62_encoded);

		std::cout << company_url << '/' << base62 << '\n';

	}

	return 0;
}