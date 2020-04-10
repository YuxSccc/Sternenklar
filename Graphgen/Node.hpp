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

    Node() = default;

    explicit Node(vector<Instruction *> &ins);

    ~Node();

    int initNode(vector<Instruction *> &ins);

private:

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
