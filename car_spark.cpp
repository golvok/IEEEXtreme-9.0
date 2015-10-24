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

struct Booking : print_printable {
	uint start;
	uint end;
	uint amount_will_pay;

	Booking() : start(0), end(0), amount_will_pay(0) { }

	void print(std::ostream& os) const {
		os << "start=" << start << ", end=" << end << ", amount=" << amount_will_pay;
	}
};

int main() {
	auto num_test_cases = get<uint>();

	for (uint test_case_num = 0; test_case_num < num_test_cases; ++test_case_num) {
		auto num_bookings = get<uint>();
		std::vector<Bookings> bookings;
		
		for (uint j = 0; j < num_bookings; ++j) {
			bookings.emplace_back();
			bookings.back().start = get<uint>();
			bookings.back().end = get<uint>();
			bookings.back().amount_will_pay = get<uint>();
		}

		std::sort(bookings.begin(),bookings.end(),[](const Booking& r, const Booking& l) -> bool {
			return r.start < l.start;
		});

		std::cout << "test case #" << test_case_num << ":\n";
		for (const auto& booking : bookings) {
			std::cout << booking << '\n';
		}

	}
	return 0;
}
