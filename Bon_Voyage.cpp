
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <locale> 
#include <limits>

using namespace std;

class Node {
public:
    bool isDoubleNode;
    Node** links;
    int* linkWeights;

    void init(int num);
};

void Node::init(int num) {
    isDoubleNode = false;
    links = new Node* [num+1];
    linkWeights = new int [num+1];
    for (int i=0; i < num+1; ++i) {
        links[i] = NULL;
        linkWeights[i] = 0;
    }
}

class Graph {
public:
    Graph(int num);
    ~Graph();

    bool addNode(int a, int b);
    int findMin();
    int findMinForNode(int nodeID);

    vector<int> doubleNodes;
    Node** nodes;

    bool twoDoubleNodesExist;

    int numNodes;
};

int Graph::findMinForNode(doubleNodes[0]) {
    ;
}

int Graph::findMin() {
    int i;
    int rtn = findMinForNode(doubleNodes[0]);
    for (i=1; i<doubleNodes.size()-1; ++i) {
        rtn = min(rtn, findMinForNode(doubleNodes[i]);
    }
    return rtn;
}

Graph::Graph(int num) {
    numNodes = num;
    nodes = new Node* [num+1];
    for (int i=0; i<num+1; ++i) {
        nodes[i] = new Node;
        nodes[i]->init(num);
    }
    twoDoubleNodesExist = false;
}

Graph::~Graph() {
    int i,j;
    for (i=0; i<numNodes+1; ++i) {
        delete [] nodes[i]->links;
        delete [] nodes[i]->linkWeights;
        delete nodes[i];
    }
    delete [] nodes;
}

bool Graph::addNode(int a, int b) {
    nodes[a]->links[b] = nodes[b];
    nodes[a]->linkWeights[b] += 1;
    if (nodes[a]->linkWeights[b] > 2) {
        // triple node
        return true;
    } else if (nodes[a]->isDoubleNode) {
        twoDoubleNodesExist = true;
    } else if (nodes[a]->linkWeights[b] > 1) {
        nodes[a]->isDoubleNode = true;
        doubleNodes.push_back(a);
    }

    nodes[b]->links[a] = nodes[a];
    nodes[b]->linkWeights[a] += 1;
    if (nodes[b]->linkWeights[a] > 2) {
        // triple node
        return true;
    } else if (nodes[b]->isDoubleNode) {
        twoDoubleNodesExist = true;
    } else if (nodes[b]->linkWeights[a] > 1) {
        nodes[b]->isDoubleNode = true;
        doubleNodes.push_back(b);
    }
    return false;
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
    vector<int> numTestCases;
    getAndParseLine(numTestCases);
    int i,j;
    bool done;
    for (i=0; i<numTestCases[0]; ++i) {
        done = false;
        vector<int> numRooms;
        getAndParseLine(numRooms);
        Graph graph(numRooms[0]);

        vector<int> numPeople;
        getAndParseLine(numPeople);
        for (j=0; j<numPeople[0]; ++j) {
            vector<int> rooms;
            getAndParseLine(rooms);
            if (!done && graph.addNode(rooms[0], rooms[1])) {
                // triple node
                cout << 2 << endl;
                done = true;
            }
        }
        
        if(!done && graph.twoDoubleNodesExist) {
            cout << 3 << endl;
            done = true;
        }

        if(!done && graph.doubleNodes.size() < 3) {
            cout << numPeople[0] << endl;
            done = true;
        }

        if(!done) {
            int ans = graph.findMin();
            cout << ans << endl;
        }

    }  
    return 0;
}