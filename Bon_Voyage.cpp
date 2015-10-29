
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
        size_t num_nodes_in_subgraph_to_try_next = numRooms;

        while (true) {
            std::vector<bool> nodes_in_subgraph(numRooms,false);
            size_t num_nodes_in_subgraph = num_nodes_in_subgraph_to_try_next;

            // fill first num_nodes_in_subgraph as true
            std::fill_n(nodes_in_subgraph.begin(), num_nodes_in_subgraph, true);

            for (uint try_num = 0; try_num < (numRooms - num_nodes_in_subgraph); ++try_num) {
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

                // shift forward by one
                nodes_in_subgraph.pop_back();
                nodes_in_subgraph.insert(nodes_in_subgraph.begin(),false);
            }

            num_nodes_in_subgraph_to_try_next -= 1;

            if (num_nodes_in_subgraph_to_try_next == 0) {
                break;
            }
        }

        std::cout << lowest_number_of_people << '\n';

    }

    return 0;
}