//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_GRAPH_HPP
#define CODESIMILARITY_GRAPH_HPP

#include "CodeSimilariry.h"
#include "Graphgen/Node.hpp"

#include <unordered_map>
#include <queue>

class Graph;

REGISTER_PTR(Graph);

class Graph {
private:
    Node *_entry;
    vector<Node *> _nodes;
    // TODO: maybe use pbds
    std::unordered_map<Node *, vector<Node *>> _edges;

public:
    enum message {
        SUCCESS,
        ERROR
    };

    Graph();

    ~Graph();

    int initGraph(Node *entry, vector<Node *> &nodes, std::unordered_map<Node *, vector<Node *>> &edges);

private:
    int isInvalid() const;
};

//----------------------------------------------------------------------------------------------------

Graph::Graph()
        : _entry(nullptr) {
}

Graph::~Graph() {
    for (Node *node : _nodes) {
        delete node;
    }
}

int Graph::initGraph(Node *entry, vector<Node *> &nodes, std::unordered_map<Node *, vector<Node *>> &edges) {
    _entry = entry;
    _nodes.swap(nodes);
    _edges.swap(edges);

    if (isInvalid()) {
        _entry = nullptr;
        for (Node *node : _nodes) {
            delete node;
        }
        _edges.clear();
        //TODO: ERR TYPE
        return ERROR;
    }
    return SUCCESS;
}

int Graph::isInvalid() const {

    return SUCCESS;
}

#endif //CODESIMILARITY_GRAPH_HPP
