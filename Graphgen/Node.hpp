//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_NODE_HPP
#define CODESIMILARITY_NODE_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Instruction.hpp"

class Node {
private:
    vector<Instruction *> _ins;

public:
    enum message {
        SUCCESS,
        ERROR
    };

public:
    Node() = default;

    explicit Node(vector<Instruction *> &ins);

    Instruction *&operator[](int _index) { return _ins[_index]; }

    ~Node();

public:

    size_t size() const { return _ins.size(); }

    Instruction *getIns(int _index) const { return _ins[_index]; }

    int initNode(vector<Instruction *> &ins);

};

//----------------------------------------------------------------------------------------------------

Node::~Node() {
    for (Instruction *ins : _ins) {
        delete ins;
    }
}

Node::Node(vector<Instruction *> &ins) {
    initNode(ins);
}

int Node::initNode(vector<Instruction *> &ins) {
    _ins.swap(ins);
    return SUCCESS;
}

#endif //CODESIMILARITY_NODE_HPP
