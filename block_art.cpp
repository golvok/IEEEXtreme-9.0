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

struct print_printable { };

template<typename T, typename STREAM>
auto operator<<(STREAM& os, const T& t) -> decltype(static_cast<const print_printable*>(&t),os) {
	t.print(os);
	return os;
}

class Rectange : public print_printable {
public:
	uint left;
	uint right;
	uint top;
	uint bottom;

	Rectange() : left(0), right(0), top(0), bottom(0) { }
	Rectange(uint left, uint right, uint top, uint bottom) : left(left), right(right), top(top), bottom(bottom) { }
	uint area() const { return (right-left+1)*(top-bottom+1); }

	void print(std::ostream& os) const {
		os << "{l=" << left << ",r=" << right << ",t=" << top << ",b=" << bottom << "; area=" << area() << '}';
	}
};

std::pair<Rectange,bool> intersection(const Rectange& r1, const Rectange& r2) {
	Rectange result;
	result.left = std::max(r1.left,r2.left);
	result.right = std::min(r1.right,r2.right);
	result.top = std::min(r1.top,r2.top);
	result.bottom = std::max(r1.bottom,r2.bottom);

	return { result, result.left < result.right && result.bottom < result.top };
}

int main() {
	auto num_rows = get<uint>(std::cin);
	auto num_columns = get<uint>(std::cin);
	(void)num_rows;
	(void)num_columns;	

	auto num_inputs = get<uint>(std::cin);

	std::vector<Rectange> additions;
	std::vector<Rectange> removals;
	std::vector<std::pair<Rectange,std::pair<uint,uint>>> queries;

	for (uint i = 0; i < num_inputs; ++i) {
		char type = get<char>(std::cin);
	
		auto bottom = get<uint>(std::cin);
		auto left = get<uint>(std::cin);
		auto top = get<uint>(std::cin);
		auto right = get<uint>(std::cin);

		Rectange r(left, right, top, bottom);
		switch (type) {
			case 'q':
				queries.emplace_back(r,std::make_pair(additions.size(),removals.size()));
				break;
			case 'a':
				additions.emplace_back(r);
				break;
			case 'r':
				removals.emplace_back(r);
				break;
		}
	}

	std::vector<unsigned long int> query_results(queries.size(), 0);
	for (size_t i = 0; i < queries.size(); ++i) {
		auto& query = queries[i].first;
		auto& query_result = query_results[i];
		// std::cout << "query:" << query << '\n';
		for (size_t j = 0; j < queries[i].second.first; ++j) {
			const auto& rect = additions[j];
			// std::cout << "\tadding:" << rect << '\n';
			auto intersection_result = intersection(query,rect);
			if (intersection_result.second) {
				// std::cout << "\t\tgood intersection:" << intersection_result.first << '\n';
				query_result += intersection_result.first.area();
			}
		}
		for (size_t j = 0; j < queries[i].second.second; ++j) {
			const auto& rect = removals[j];
			// std::cout << "\tremoving:" << rect << '\n';
			auto intersection_result = intersection(query,rect);
			if (intersection_result.second) {
				// std::cout << "\t\tgood intersection:" << intersection_result.first << '\n';
				query_result -= intersection_result.first.area();
			}
		}
	}

	for (const auto& query_result : query_results) {
		std::cout << query_result << '\n';
	}

	return 0;
}