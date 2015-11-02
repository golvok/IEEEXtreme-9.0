
#include "common/generator.hpp"
#include "common/geometry.hpp"

#include <vector>
#include <cstdlib>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <locale>
#include <limits>
#include <memory>

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

template<typename ID_TYPE, typename TAG>
class ID {
public:
	using id_type = ID_TYPE;
private:
    id_type value;
public:
    explicit ID(const id_type& value) : value(value) { }
    operator id_type() const { return value; }
};

template<typename ID_TYPE, typename TAG> bool operator< (const ID<ID_TYPE,TAG>& rhs, const ID<ID_TYPE,TAG>& lhs) {return ((ID_TYPE)rhs) <  ((ID_TYPE)lhs); }
template<typename ID_TYPE, typename TAG> bool operator==(const ID<ID_TYPE,TAG>& rhs, const ID<ID_TYPE,TAG>& lhs) {return ((ID_TYPE)rhs) == ((ID_TYPE)lhs); }
template<typename ID_TYPE, typename TAG> bool operator!=(const ID<ID_TYPE,TAG>& rhs, const ID<ID_TYPE,TAG>& lhs) {return ((ID_TYPE)rhs) != ((ID_TYPE)lhs); }


template<typename CONTAINER>
struct cyclic_get_adaptor {
	CONTAINER& c;
	cyclic_get_adaptor(CONTAINER& c) : c(c) { }
	template<typename INTEGRAL>
	auto& operator[](INTEGRAL i)       { return c[ (i < 0) ? (c.size() - std::abs((std::ptrdiff_t)i) % c.size()) : (i % c.size()) ]; }
	template<typename INTEGRAL>
	auto& operator[](INTEGRAL i) const { return c[ (i < 0) ? (c.size() - std::abs((std::ptrdiff_t)i) % c.size()) : (i % c.size()) ]; }
};

template<typename CONTAINER>
auto cyclic_access(CONTAINER& c) {
	return cyclic_get_adaptor<CONTAINER>(c);
}


struct tree_type_tag {};
using TreeID = ID<size_t,tree_type_tag>;

using Point = geom::Point<int>;
using Line = std::pair<Point,Point>;

enum class Direction : unsigned char {
	RIGHT = 0, UP = 1, LEFT = 2, DOWN = 3,
};

enum class Orientation : unsigned char {
	HORIZONTAL = 0, VERTICAL = 1,
};

const Point& theMostDirPoint(const Line& l, Direction dir) {
	switch (dir) {
		default:
		case Direction::RIGHT:
			if (l.first.x > l.second.x) {
				return l.first;
			} else if (l.first.x < l.second.x) {
				return l.second;
			} else {
				return l.first;
			}
		case Direction::UP:
			if (l.first.y > l.second.y) {
				return l.first;
			} else if (l.first.y < l.second.y) {
				return l.second;
			} else {
				return l.first;
			}
		case Direction::LEFT:
			if (l.first.x < l.second.x) {
				return l.first;
			} else if (l.first.x > l.second.x) {
				return l.second;
			} else {
				return l.second;
			}
		case Direction::DOWN:
			if (l.first.y < l.second.y) {
				return l.first;
			} else if (l.first.y > l.second.y) {
				return l.second;
			} else {
				return l.second;
			}
	}
}

Direction flip(Direction dir) {
	switch (dir) {
		default:
		case Direction::RIGHT:
			return Direction::LEFT;
		case Direction::UP:
			return Direction::DOWN;
		case Direction::LEFT:
			return Direction::RIGHT;
		case Direction::DOWN:
			return Direction::UP;
	}
}

std::ostream& operator<<(std::ostream& os, const Direction& dir) {
	switch (dir) {
		default:
		case Direction::RIGHT:
			return os << "RIGHT";
		case Direction::UP:
			return os << "UP";
		case Direction::LEFT:
			return os << "LEFT";
		case Direction::DOWN:
			return os << "DOWN";
	}
	return os;
}

Orientation orientationOfDirection(Direction dir) {
	switch (dir) {
		default:
		case Direction::RIGHT:
		case Direction::LEFT:
			return Orientation::HORIZONTAL;
		case Direction::UP:
		case Direction::DOWN:
			return Orientation::VERTICAL;
	}
}

auto getCordCorrispondingTo(const Point& p, Orientation orien) {
	switch (orien) {
		default:
		case Orientation::HORIZONTAL:
			return p.x;
		case Orientation::VERTICAL:
			return p.y;
	}
}

template<typename CONTAINER>
double polygonArea(const CONTAINER& points, size_t begin, size_t past_end, Point point_before_first) {

	double area = 0;
	const auto& cyc_points = cyclic_access(points);

	Point prev = point_before_first;
	for (auto i = begin; i < past_end; i++) {
		Point curr = cyc_points[i];
		double inc_area = (curr.x + prev.x) * (curr.y - prev.y);
		// std::cout << "\tadding: (" << curr.x << " + " << prev.x << ") * (" << curr.y << " - " << prev.y << ") = " << inc_area << "\n";
		area += inc_area;
		prev = curr;
	}

	return area * 0.5;
}

class TreeManager {
public:
	TreeManager()
		: caches_valid(true)
		, all_trees()
		, sorted_trees()
	{ }

	void addTree(const Point& tree) {
		caches_valid = false;
		all_trees.emplace_back(tree);
	}

	auto& getSortedTrees(Orientation orien) { return sorted_trees[(size_t)orien]; }

	void buildCaches() {
		for (auto pair : {
			std::make_pair(Orientation::HORIZONTAL, std::ref(getSortedTrees(Orientation::HORIZONTAL))),
			std::make_pair(Orientation::VERTICAL, std::ref(getSortedTrees(Orientation::VERTICAL)))
		}) {
			auto& to_be_sorted = pair.second;
			to_be_sorted.clear();
			std::copy(all_trees.begin(), all_trees.end(), std::back_inserter(to_be_sorted));
			std::sort(to_be_sorted.begin(), to_be_sorted.end(), [&](const Point& l, const Point& r) {
				switch (pair.first) {
					default:
					case Orientation::HORIZONTAL:
						return l.x < r.x;
					case Orientation::VERTICAL:
						return l.y < r.y;
				}
			});
		}
		caches_valid = true;
	}

	bool noTreesInDirection(const Point& p, Direction dir) {
		switch (dir) {
			default:
			case Direction::RIGHT:
				return p.x > getSortedTrees(orientationOfDirection(dir)).back().x;
			case Direction::UP:
				return p.y > getSortedTrees(orientationOfDirection(dir)).back().y;
			case Direction::LEFT:
				return p.x < getSortedTrees(orientationOfDirection(dir)).front().x;
			case Direction::DOWN:
				return p.y < getSortedTrees(orientationOfDirection(dir)).front().y;
		}
	}

	auto boundsToSearch(const Line& line, Orientation orien) {
		Direction dir_of_neg_inf = [&]() {
			switch(orien) {
				default:
				case Orientation::HORIZONTAL:
					// std::cout << "\t\tsearching HORIZONTAL\n";
					return Direction::LEFT;
				case Orientation::VERTICAL:
					// std::cout << "\t\tsearching VERTICAL\n";
					return Direction::DOWN;
			}
		}();

		// std::cout << "\t\tlooking at trees between " << theMostDirPoint(line,     dir_of_neg_inf ) << " and " << theMostDirPoint(line,flip(dir_of_neg_inf)) << '\n';

		auto& to_search = getSortedTrees(orien);
		return std::make_pair(
			std::lower_bound(to_search.begin(),to_search.end(),theMostDirPoint(line,     dir_of_neg_inf ),[&](const auto& l, const auto& r) {
				return getCordCorrispondingTo(l,orien) < getCordCorrispondingTo(r,orien);
			}),
			std::upper_bound(to_search.begin(),to_search.end(),theMostDirPoint(line,flip(dir_of_neg_inf)),[&](const auto& l, const auto& r) {
				return getCordCorrispondingTo(l,orien) < getCordCorrispondingTo(r,orien);
			})
		);
	}

	bool allTreesAreToTheRight(const Line& line) {
		std::pair<decltype(all_trees.begin()),decltype(all_trees.begin())> range;

		if (caches_valid) {

			const auto left_of_line_is_actually = [&]() -> std::pair<Direction,Direction> {
				const auto slope = [](Point p) { return p.y/(double)p.x; }(line.second - line.first);
				if (line.first.x < line.second.x) {
					if (slope > 1) {
						return { Direction::LEFT, Direction::UP };
					} else if (slope > 0) {
						return { Direction::UP, Direction::LEFT };
					} else if (slope < -1) {
						return { Direction::RIGHT, Direction::UP };
					} else {
						return { Direction::UP, Direction::RIGHT };
					}
				} else {
					if (slope > 1) {
						return { Direction::LEFT, Direction::DOWN };
					} else if (slope > 0) {
						return { Direction::DOWN, Direction::LEFT };
					} else if (slope < -1) {
						return { Direction::RIGHT, Direction::DOWN };
					} else {
						return { Direction::DOWN, Direction::RIGHT };
					}
				}
			}();

			// std::cout << "\t\"left\" is " << left_of_line_is_actually.first << " but " << left_of_line_is_actually.second << '\n';

			for (auto dir : { left_of_line_is_actually.first, left_of_line_is_actually.second }) {
				if (noTreesInDirection(theMostDirPoint(line,flip(dir)),dir)) {
					// std::cout << "\tno trees anywhere remotely " << dir << "\n";
					return true;
				} else if (!noTreesInDirection(theMostDirPoint(line,dir),dir)) {
					// std::cout << "\tdefinitely trees to the " << flip(dir) << "\n";
					return false;
				}
			}

			// std::cout << "\tresorting to search\n";
			range = boundsToSearch(line, orientationOfDirection(left_of_line_is_actually.first));

		} else {
			// std::cout << "\tno chaches yet\n";
			range = std::make_pair(all_trees.begin(),all_trees.end());
		}

		return std::find_if(range.first, range.second, [&](const auto& tree) {
			auto first_to_second = line.second - line.first;
			auto first_to_tree = tree - line.first;
			// std::cout << "\t\tlooking at tree " << tree;
			if (((first_to_second.x)*(first_to_tree.y) - (first_to_second.y)*(first_to_tree.x)) >= 0) {
				// std::cout << " - it was not on the left!\n";
				return true;
			} else {
				// std::cout << '\n';
				return false;
			}
		}) == range.second;
	}

	const auto& trees() { return all_trees; }
private:
	bool caches_valid;
	std::vector<Point> all_trees;

	std::array<std::vector<Point>,2> sorted_trees;
};

int main() {
	const auto numTestCases = get<uint>();
	for (uint test_case_num = 0; test_case_num < numTestCases; ++test_case_num ) {
		// std::cout << "=== Test Case #" << test_case_num << " ===\n";

		std::vector<Point> border; // will be in CCW order
		const auto numBorderPoints = get<uint>();
		int max_x = 0;
		int max_y = 0;
		int min_x = 0;
		int min_y = 0;
		for (uint i = 0; i < numBorderPoints; ++i) {
			const auto x = get<int>();
			const auto y = get<int>();
			border.emplace_back(x,y);
			max_x = std::max(x,max_x);
			max_y = std::max(y,max_y);
			min_x = std::min(x,min_x);
			min_y = std::min(y,min_y);
		}

		TreeManager trees;
		const auto numTrees = get<uint>();
		for (uint i = 0; i < numTrees; ++i) {
			const auto x = get<int>();
			const auto y = get<int>();
			trees.addTree(Point(x,y));
		}

		// for (auto y : xrange<int>(max_y,min_y)) {
		// 	std::cout << y << ":";
		// 	for (auto x : xrange<int>(min_x,max_x)) {
		// 		if (std::find(border.begin(),border.end(), Point(x,y)) == border.begin()) {
		// 			std::cout << '0';
		// 		} else if (std::find(border.begin(),border.end(), Point(x,y)) != border.end()) {
		// 			std::cout << 'B';
		// 		} else if (std::find(trees.trees().begin(),trees.trees().end(),Point(x,y)) != trees.trees().end()) {
		// 			std::cout << 'T';
		// 		} else {
		// 			std::cout << ' ';
		// 		}
		// 	}
		// 	std::cout << '\n';
		// }
		// std::cout << "  ";
		// for (auto x : xrange<int>(min_x,max_x)) {
		// 	std::cout << x;
		// }
		// std::cout << '\n';

		if (numTrees == 0) {
			std::cout << polygonArea(border,0,border.size(),border.back()) << '\n';
			return 0;
		}

		trees.buildCaches();

		double best_area = 0;
		std::pair<int,int> best_indexes = {0,0};

		size_t other_index = 0;
		for (size_t i_border_point = 0; i_border_point < border.size(); ++i_border_point) {
			other_index = std::max(other_index,i_border_point+2);
			while (true) {
				// if ((other_index + 1) % border.size() == i_border_point) {
				// 	break; // loop around condition. confuses new tree checking
				// }

				Line line(
					cyclic_access(border)[other_index],
					cyclic_access(border)[i_border_point]
				);

				// std::cout << "trying " << line.first << "->" << line.second << " (" << other_index << "->" << i_border_point << ")\n";
				if (!trees.allTreesAreToTheRight(line)) {
					// std::cout << "\tbut there were trees\n\n";
					break;
				}
				double area = polygonArea(border, i_border_point, other_index+1, line.first);
				// std::cout << "\tarea=" << area << '\n';
				if (area > best_area) {
					// std::cout << "\tit was better\n";
					best_area = area;
					best_indexes = {other_index,i_border_point};
				}
				other_index += 1;
			}
		}

		// std::cout << "best cut: " << best_indexes.first << ',' << best_indexes.second << "\nFINAL ANSWER: ";
		std::cout << best_area << '\n';
	}
}
