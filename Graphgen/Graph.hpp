//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_GRAPH_HPP
#define CODESIMILARITY_GRAPH_HPP

#include "CodeSimilariry.h"
#include "Graphgen/Node.hpp"

#include <unordered_map>
#include <queue>
#include <map>
#include <algorithm>

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

public:
    size_t size() const { return _nodes.size(); }

    int initGraph(Node *entry, vector<Node *> &nodes, std::unordered_map<Node *, vector<Node *>> &edges);

    vector<std::pair<int, int>> GetCurrentIndexEdge() const {
        // Not Safe
        vector<std::pair<int, int>> _edgePair;
        std::map<Node *, int> _indexMap;
        for (int i = 0; i < _nodes.size(); ++i) {
            _indexMap[_nodes[i]] = i;
        }
        for (auto &it : _nodes) {
            auto _ENode = _edges.find(it);
            if (_ENode != _edges.end()) {
                for (auto &terminal : _ENode->second) {
                    _edgePair.emplace_back(std::make_pair(_indexMap[it], _indexMap[terminal]));
                }
            }
        }
        return _edgePair;
    }

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
