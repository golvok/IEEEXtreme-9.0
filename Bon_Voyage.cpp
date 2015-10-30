
#include "common/generator.hpp"

#include <vector>
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

std::pair<std::vector<bool>,bool> first_number_with_n_bits(size_t num_bits, size_t num_high_bits) {
    std::vector<bool> result(num_bits,false);
    std::fill_n(result.begin(), num_high_bits, true);
    return {result, num_high_bits <= num_bits};
}

std::pair<std::vector<bool>,bool> next_number_with_same_number_of_bits(std::vector<bool>&& gen_number) {

    auto first_one = std::find(gen_number.begin(), gen_number.end(), true);
    auto next_zero = std::find(first_one, gen_number.end(), false);
    size_t run_size = std::distance(first_one,next_zero);

    if (next_zero != gen_number.end()) {
        *next_zero = 1;
    }

    std::fill_n(gen_number.begin(), (run_size-1), true);
    std::fill(gen_number.begin() + (run_size-1), next_zero, false);

    return {gen_number,next_zero != gen_number.end()};
}

template<typename id_type, typename TAG>
class ID {
    id_type value;
public:
    explicit ID(const id_type& value) : value(value) { }
    operator id_type() const { return value; }
};

struct NodeID_tag {};
struct EdgeID_tag {};
using NodeID = ID<size_t,NodeID_tag>;
using EdgeID = ID<size_t,EdgeID_tag>;
const NodeID INVALID_NODE = NodeID((size_t)-1);
const EdgeID INVALID_EDGE = EdgeID((size_t)-1);

class Node {
public:
    Node() : out_edges() { }
    std::vector<EdgeID> out_edges;
    void addEdge(EdgeID eid) { out_edges.push_back(eid); }
};

class Graph {
public:
    Graph(size_t num_nodes)
        : nodes(num_nodes)
        , edges()
    {
        for (auto& node : nodes) {
            node = std::make_unique<Node>();
        }
    }

    Graph& operator=(const Graph&) = delete;
    Graph(const Graph&) = delete;

    void addEdge(NodeID a, NodeID b) {
        edges.emplace_back(a,b);
        nodes[a]->addEdge(EdgeID(edges.size() - 1));
        nodes[b]->addEdge(EdgeID(edges.size() - 1));
    }

    size_t BFS(NodeID nodeID);

    NodeID getTarget(EdgeID eid, NodeID src_id) {
        const auto& edge = edges[eid];
        return (edge.first == src_id) ? edge.second : edge.first;
    }

    auto nodeIDs() { return xrange<size_t>(0, nodes.size()-1); }
    auto edgeIDs() { return xrange<size_t>(0, edges.size()-1); }

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<std::pair<NodeID,NodeID>> edges;
};

size_t Graph::BFS(NodeID nodeID) {
    std::vector<size_t> distance(nodes.size(),(size_t)-1);

    std::vector<NodeID> current_queue;
    std::vector<NodeID> next_queue;
    next_queue.push_back(nodeID);

    while (next_queue.empty() == false) {
        std::swap(current_queue,next_queue);

        while (current_queue.empty() == false) {

            NodeID current_node = current_queue.back();
            current_queue.pop_back();

            for (EdgeID eid : nodes[current_node]->out_edges) {
                NodeID other_node = getTarget(eid, current_node);

                if (distance[other_node] == (size_t)(-1)) {
                    distance[other_node] = distance[current_node] + 1;
                    next_queue.push_back(other_node);
                }
            }
        }
    }

    return 0;
}

template <typename T>
unsigned int getAndParseLine(std::vector<T>& elements, char delim=' ', std::istream& stream=std::cin){
    // Get line
    std::string line;
    getline(stream, line);

    // Get each element in line
    std::string element;
    std::stringstream ss(line);
    while (std::getline(ss, element, delim)) {
        std::stringstream converter(element);
        T convertedElement;
        converter >> convertedElement;
        elements.push_back(convertedElement);
    }

    return elements.size();
}

int main() {
    const auto numTestCases = get<uint>();
    for (uint i=0; i<numTestCases; ++i) {
        const auto numRooms = get<uint>(); 
        Graph graph(numRooms);

        const auto numPeople = get<uint>();
        for (uint j=0; j<numPeople; ++j) {
            auto room0 = get<uint>();
            auto room1 = get<uint>();
            // std::cout << "room: " << room0 << " " << room1 << '\n';
            graph.addEdge(NodeID(room0-1), NodeID(room1-1));
        }

        uint lowest_number_of_people = numPeople;
        size_t num_nodes_in_subgraph_to_try_next = 2;

        while (true) {
            const size_t num_nodes_in_subgraph = num_nodes_in_subgraph_to_try_next;

            // iterate over every subgraph of size num_nodes_in_subgraph
            for (
                auto nodes_in_subgraph_pair = first_number_with_n_bits(numRooms, num_nodes_in_subgraph);
                nodes_in_subgraph_pair.second == true;
                nodes_in_subgraph_pair = next_number_with_same_number_of_bits(std::move(nodes_in_subgraph_pair.first))
            ) {
                const auto& nodes_in_subgraph = nodes_in_subgraph_pair.first; // a bitset with 1s where we should include the nodes
                size_t num_edges_in_subgraph = 0;

                // std::cout << "subgraph:\n";

                for (auto eid : graph.edgeIDs()) {
                    if (   nodes_in_subgraph[graph.edges[eid].first]
                        && nodes_in_subgraph[graph.edges[eid].second]
                    ) {
                        num_edges_in_subgraph += 1;
                        // std::cout << '\t' << graph.edges[eid].first+1 << ',' << graph.edges[eid].second+1 << '\n';
                    }
                }
                int eu_char = num_nodes_in_subgraph - num_edges_in_subgraph;
                // std::cout << "Euler Characteristic = " << eu_char << '\n';

                if (eu_char < 0) {
                    if (num_nodes_in_subgraph < lowest_number_of_people) {
                        lowest_number_of_people = num_nodes_in_subgraph;
                    }
                }
            }

            num_nodes_in_subgraph_to_try_next += 1;

            if (num_nodes_in_subgraph_to_try_next > numRooms) {
                break;
            }

            if (num_nodes_in_subgraph_to_try_next > lowest_number_of_people) {
                break;
            }
        }

        // std::cout << "FINAL ANSWER: ";
        std::cout << lowest_number_of_people << '\n';

    }

    return 0;
}