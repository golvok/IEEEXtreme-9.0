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

int main() {
	auto num_test_cases = get<size_t>(std::cin);
	for (size_t test_case = 0; test_case < num_test_cases; ++test_case) {
		const auto num_players = get<size_t>(std::cin);
		const auto num_competitions = get<size_t>(std::cin);

		// std::cout << "num_competitions=" << num_competitions << ", num_players=" << num_players << '\n';

		std::vector<std::string> player_names;
		std::vector<std::vector<std::pair<short,short>>> player_predictions;
		std::vector<std::pair<short,short>> competition_results;

		for (size_t i = 0; i < num_players; ++i) {
			if (std::cin.eof()) {
				break;
			}
			player_names.push_back(get<std::string>(std::cin));
			player_predictions.emplace_back();
			if (std::cin.eof()) {
				break;
			}
			for (size_t j = 0; j < num_competitions; ++j) {
				auto p1 = get<short>(std::cin);
				auto p2 = get<short>(std::cin);

				player_predictions.back().emplace_back(p1,p2);
			}
		}

		for (size_t i = 0; i < num_competitions; ++i) {
			auto c = get<char>(std::cin);
			// std::cout << c << ' ';
			if (std::cin.eof()) {
				break;
			} else if (c == '?') {
				competition_results.emplace_back(-1,-1);
				c = get<char>(std::cin); // get second question mark
			} else if (std::isalpha(c)) {
				std::cin.unget(); // put it back
				break; // go to next player
			} else {
				std::cin.unget(); // put thing that is not a question mark or letter back
				auto p1 = get<short>(std::cin);
				auto p2 = get<short>(std::cin);

				competition_results.emplace_back(p1,p2);
			}
		}

		// for (size_t i = 0; i < num_players; ++i) {
		// 	std::cout << player_names[i] << '\n';			
		// 	for (size_t j = 0; j < num_competitions; ++j) {
		// 		std::cout << player_predictions[i][j].first << ' ' << player_predictions[i][j].second << ", ";
		// 	}
		// 	std::cout << '\n';
		// }

		// std::cout << "competition results:\n";
		// for (size_t i = 0; i < num_competitions; ++i) {
		// 	std::cout << competition_results[i].first << ' ' << competition_results[i].second << ", ";
		// }
		// std::cout << '\n';
		using t_score = unsigned long;

		auto winner = [](const std::pair<short,short>& p) -> int {
			return p.first < p.second;
		};

		std::vector<t_score> scores(num_players,0);
		
		for (size_t player_index = 0; player_index < num_players; ++player_index) {
			auto& player_score = scores[player_index];
			for (size_t comp_index = 0; comp_index < num_competitions; ++comp_index) {
				const auto& player_prediction = player_predictions[player_index][comp_index];
				const auto& competition_result = competition_results[comp_index];

				if (competition_result.first == -1) {
					continue; // skip not yet determined
				}

				if (winner(player_prediction) == winner(competition_result)) {
					player_score += 10;
				}

				player_score += std::max(0,5 - std::abs(player_prediction.first - competition_result.first));
				player_score += std::max(0,5 - std::abs(player_prediction.second - competition_result.second));

				player_score += std::max(0,5 - std::abs((player_prediction.first - player_prediction.second) - (competition_result.first - competition_result.second)));
			}
		}

		for (size_t i = 0; i < num_players; ++i) {
			std::cout << player_names[i] << " : " << scores[i] << '\n';
		}

		std::vector<std::string> winning_names;

		auto first_it = std::max_element(scores.begin(),scores.end());
		auto max_it = first_it;
		while (true) {
			if (max_it == scores.end()) {
				break;
			}
			winning_names.push_back(player_names[max_it - scores.begin()]);
			max_it = std::find(max_it+1,scores.end(),*first_it);
		}

		std::sort(winning_names.begin(),winning_names.end());

		for (const auto& name : winning_names) {
			std::cout << name << ' ';
		}
		std::cout << '\n';
	}
}
