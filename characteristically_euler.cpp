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

char asLetter(bool is_black, uint id) {
	if (id == (uint)(-1)) {
		return '?';
	}
	return (is_black ? 'A' : 'a') + id;
}

int main() {
	auto num_test_cases = get<uint>();

	for (uint test_case_num = 0; test_case_num < num_test_cases; ++test_case_num) {
		const uint height = get<uint>();
		const uint width = get<uint>();

		std::vector<std::vector<bool>> image;
		image.reserve(height + 2);
		image.emplace_back(width + 2);
		for (uint i = 0; i < height; ++i) {
			image.emplace_back(width + 2);
			for (uint j = 0; j < width; ++j) {
				image.back()[j+1] = (get<char>() == 'X');
			}
		}
		image.emplace_back(width + 2);

		std::array<std::unordered_map<uint,uint>,2> connection_sets;
		std::array<uint,2> num_areas;
		num_areas[0] = 1;
		num_areas[1] = 0;
		
		std::vector<std::vector<uint>> fills;
		fills.reserve(height + 2);

		fills.emplace_back(width + 2, 0);
		for (uint i = 0; i < (height+2); ++i) {
			fills.emplace_back(width + 2, (uint)-1);
			fills.back()[0] = 0;
			fills.back()[width + 1] = 0;
		}
		fills.emplace_back(width + 2, 0);

		for (int i = 0; i < (int)height; ++i) {
			int row = i+1;
			for (int j = 0; j < (int)width; ++j) {
				int col = j+1;
				const auto image_val = image[row][col];
				uint id_to_fill = num_areas[image_val];

				// std::cout << "filling {" << row << ',' << col << "} (" << image_val << ")\n";

				auto func = [&](int l_row, int l_col) {
					const uint l_id = fills[l_row][l_col];
					const auto l_image_val = image[l_row][l_col];
					// bool looking_at_boundary = l_row < 1 || l_col < 1 || l_row > (int)height || l_col > (int)width;
					// std::cout << "\tlooking at {" << l_row << ',' << l_col << "} (" << l_image_val << ") l_id=" << asLetter(l_image_val,l_id) << ", ib=" << looking_at_boundary;
					if (l_id != (uint)-1 && l_image_val == image_val) {
						// std::cout << " examining.";
						if (id_to_fill > l_id) {
							// std::cout << " was a better l_id";
							if (id_to_fill != num_areas[image_val]) {
								// std::cout << " connected. adding " << asLetter(image_val,id_to_fill) << " -> " << asLetter(l_image_val,l_id);
								// std::cout << '\n';
								auto find_results = connection_sets[image_val].find(id_to_fill);
								if (find_results != connection_sets[image_val].end() && find_results->second != l_id) {
									std::cout << "!!!!!!!!!!!!!!!!! Double Mapping !!!!!!!!!!!!!!!!\nExiting...\n";
									exit(1);
								}
								connection_sets[image_val].emplace(id_to_fill,l_id);
							}
							id_to_fill = l_id; 
						}
					}
					// std::cout << '\n';
				};

				for (const auto& coord : {
					std::make_pair(row-1, col+0),
					std::make_pair(row+1, col+0),
					std::make_pair(row+0, col-1),
					std::make_pair(row+0, col+1),
				}) {
					func(coord.first,coord.second);
				}

				if (image_val) {
					for (const auto& coord : {
						std::make_pair(row-1, col-1),
						std::make_pair(row-1, col+1),
						std::make_pair(row+1, col-1),
						std::make_pair(row+1, col+1),
					}) {
						func(coord.first, coord.second);
					}
				}

				fills[row][col] = id_to_fill;
				// std::cout << "\tfilling with " << id_to_fill << '\n';

				if (id_to_fill == num_areas[image_val]) {
					// std::cout << "\tincrementing highest val\n";
					num_areas[image_val] += 1;
				}
			}
		}

		std::cout << "input image:\n";
		for (uint i = 0; i < height; ++i) {
			std::cout << '\t';
			for (uint j = 0; j < width; ++j) {
				std::cout << (image[i+1][j+1] ? "B" : "-");
			}
			std::cout << '\n';
		}

		std::cout << '\n';

		std::cout << "final colouring:\n";
		for (uint colour : { 0, 1 }) {
			std::cout << (colour ? "\tblack:\n" : "\twhite:\n");
			for (uint i = 0; i < height; ++i) {
				std::cout << "\t\t";
				for (uint j = 0; j < width; ++j) {
					if (image[i+1][j+1] == colour) {
						std::cout << asLetter(image[i+1][j+1],fills[i+1][j+1]);
					} else {
						std::cout << '-';
					}
				}
				std::cout << '\n';
			}
		}

		std::cout << "connections:\n";
		for (uint i : { 0, 1 }) {
			const auto& map = connection_sets[i];
			std::cout << (i ? "\tblack:\n" : "\twhite:\n");
			for (const auto& value : map) {
				std::cout << "\t\t" << asLetter(i,value.first) << " -> " << asLetter(i,value.second) << '\n';
				num_areas[i] -= 1;
			}
		}

		std::cout << "final asnswer:\n\t= (numBlackAreas = " << (int)num_areas[1] << ") - (numWhiteAreas = " << (int)num_areas[0] << ") + 1\n\t= ";
		std::cout << (int)num_areas[1] - (int)num_areas[0] + 1 << '\n';

	}

	return 0;
}
