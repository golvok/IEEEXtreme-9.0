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

using InputBitset = std::bitset<10>;

struct Expression : public print_printable {
	InputBitset mask;
	InputBitset values;

	Expression() : mask(), values() { }
	Expression(Expression&&) = default;

	bool matches(const InputBitset& test) const {
		return (test & mask) == values;
	}

	bool operator==(const Expression& rhs) const {
		return (this->mask == rhs.mask) && (this->values == rhs.values);
	}

	void print(std::ostream& os) const {
		os << "mask=" << mask.to_string() << " values=" << values.to_string();
	}
};

namespace std {
	template<>
	struct hash<Expression>{
		size_t operator()(const Expression& e) const {
			return std::hash<unsigned long long>()(e.mask.to_ullong()) ^ std::hash<unsigned long long>()(e.values.to_ullong() << 10);
		}
	};
}

class State : public print_printable {
private:
	uint id;
	uint output;
	std::vector<std::pair<Expression,uint>> transitions;

public:
	State(uint id, uint output) : id(id), output(output), transitions() { }

	void addState(Expression&& expr, uint next_state) {
		transitions.emplace_back(std::move(expr),next_state);
	}

	uint getNextState(const InputBitset& input) {
		for (const auto& trans : transitions) {
			if (trans.first.matches(input)) {
				return trans.second;
			}
		}
		return id;
	}

	uint getOutput() const { return output; }

	void print(std::ostream& os) const {
		os << "state " << id << ", output=" << output;
		for (const auto& trans : transitions) {
			os << "\n\t" << trans.first << " -> " << trans.second;
		}
	}
};

int main() {
	auto num_states = get<uint>();
	auto num_inputs = get<uint>();

	std::vector<State> states;
	for (uint i = 0; i < num_states; ++i) {
		auto output = get<uint>();
		auto num_transitions = get<uint>();

		states.emplace_back(i,output);
		for (uint j = 0; j < num_transitions; ++j) {
			auto trans_string = get<std::string>();
			uint num_transitions_in_this_state = (trans_string.size()-2)/4 + 1;

			Expression new_expr;
			for (uint k = 0; k < num_transitions_in_this_state; ++k) {
				size_t subexpress_offset = k*4;
				uint input = trans_string[subexpress_offset + 0] - 'A';
				uint value = trans_string[subexpress_offset + 2] - '0';

				new_expr.mask[input] = 1;
				new_expr.values[input] = value;
			}

			uint next_state = trans_string.back() - '0';
			states.back().addState(std::move(new_expr),next_state);
		}
	}


	auto num_ticks = get<uint>();
	auto current_state = get<uint>();

	std::vector<InputBitset> inputs_at_tick;
	for (uint i = 0; i < num_ticks; ++i) {
		inputs_at_tick.emplace_back();
		for (uint j = 0; j < num_inputs; ++j) {
			inputs_at_tick.back()[j] = get<uint>();
		}
	}

	// for (const auto& state : states) {
	// 	std::cout << state << '\n';
	// }

	// std::cout << "num_ticks=" << num_ticks << '\n';
	// std::cout << "current_state=" << current_state << '\n';

	// for (const auto& inputs : inputs_at_tick) {
	// 	std::cout << inputs.to_string() << '\n';
	// }

	std::vector<uint> states_at_tick;
	for (const auto& inputs : inputs_at_tick) {
		current_state = states[current_state].getNextState(inputs);
		states_at_tick.push_back(current_state);
	}

	// for (const auto& state : states_at_tick) {
	// 	std::cout << state << '\n';
	// }

	const size_t chunk_size = 16;
	const size_t lines_per_chunk = num_inputs + 4;
	const size_t num_chunks = (num_ticks-1)/chunk_size + 1;
	std::vector<std::string> lines(lines_per_chunk*num_chunks);
	for (uint tick = 0; tick < num_ticks; ++tick) {
		auto base_line = lines.begin() + (tick/chunk_size * lines_per_chunk) + 1;
		if (tick % chunk_size == 0) {
			base_line[-1] = "Tick #" + std::to_string(tick+1);
			base_line[num_inputs + 0] += "OUT   ";
			base_line[num_inputs + 1] += "STATE ";
		}
		for (uint input_index = 0; input_index < num_inputs; ++input_index) {
			if (tick % chunk_size == 0) {
				base_line[input_index] += (char)('A' + input_index);	
				base_line[input_index] += "     ";
			}
			if (inputs_at_tick[tick][input_index]) {
				base_line[input_index] += "***";
			} else {
				base_line[input_index] += "___";
			}
		}
		base_line[num_inputs + 0] += states[states_at_tick[tick]].getOutput()
			? "***" : "___";
		base_line[num_inputs + 1] += "  " + std::to_string(states_at_tick[tick]);
	}

	for (const auto& line : lines) {
		std::cout << line << '\n';
	}

	return 0;
}
