#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

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

using ytype = unsigned long long int;
using xtype = unsigned short;
using cow_type = std::pair<ytype,xtype>;

namespace std {
	template<>
	struct hash<cow_type> {
		size_t operator()(const cow_type& cow) const {
			return std::hash<xtype>()(cow.first) ^ std::hash<ytype>()(cow.second);
		}
	};
}

class CowLocations {
public:
private:
	std::unordered_set<cow_type> cows;
public:
	CowLocations() : cows() { }

	void addCow(xtype x, ytype y) {
		cows.emplace(x,y);
	}

	bool hasCowAt(xtype x, ytype y) {
		return cows.find(cow_type(x,y)) != cows.end();
	}
};

int main() {
	auto road_length = get<xtype>(std::cin);
	auto num_cows = get<uint>(std::cin);

	CowLocations cows;
	for (uint i = 0; i < num_cows; ++i) {
		auto x = get<xtype>(std::cin);
		auto y = get<ytype>(std::cin);
		cows.addCow(x,y);
	}

	// usage:
	// cows.hasCowAt(x,y);
}