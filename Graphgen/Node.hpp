//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_NODE_HPP
#define CODESIMILARITY_NODE_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Instruction.hpp"

namespace ster {

    class Node {
    private:
        vector<Instruction *> _ins;
        std::string _name;
        vector<std::pair<int, string>> _source_code;

    public:
        enum message {
            SUCCESS,
            ERROR
        };

    public:
        Node() = default;

        inline explicit Node(vector<Instruction *> &ins, string &_tName, vector<std::pair<int, string>> _source_vector);

        Instruction *&operator[](int _index) { return _ins[_index]; }

        inline ~Node();

    public:
        size_t size() const { return _ins.size(); }

        Instruction getIns(int _index) const { return *_ins[_index]; }

        inline int initNode(vector<Instruction *> &ins);

        const vector<std::pair<int, string>> &get_source_code() const { return _source_code; }
    };

//----------------------------------------------------------------------------------------------------

    Node::~Node() {
        for (Instruction *ins : _ins) {
            delete ins;
        }
    }

    Node::Node(vector<Instruction *> &ins, string &_tName, vector<std::pair<int, string>> _source_vector) {
        initNode(ins);
        _name = _tName;
        _source_code = std::move(_source_vector);
    }

    int Node::initNode(vector<Instruction *> &ins) {
        _ins.swap(ins);
        return SUCCESS;
    }
}
#endif //CODESIMILARITY_NODE_HPP
