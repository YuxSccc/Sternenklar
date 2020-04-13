//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_INSTRUCTION_HPP
#define CODESIMILARITY_INSTRUCTION_HPP

#include "CodeSimilariry.h"
#include "Graphgen/Value.hpp"

#include "llvm/IR/Instruction.h"

class Instruction {
public:
    enum class Type {
        CallInst,
        ArithInst,
        BrInst,
        Unknown
    };

private:
    Type _type;
    vector<Value> _params;
    std::string _llvm_name;

public:
    Instruction() = default;

    explicit Instruction(const llvm::Instruction &ins);

    ~Instruction() = default;

public:
    inline Type getType() const { return _type; };

    inline size_t paramSize() const { return _params.size(); }

    const Value &getParam(size_t _index) const;

    vector<Value> getParamsCopy() const {
        return vector<Value>(_params);
    }

};

//----------------------------------------------------------------------------------------------------

Instruction::Instruction(const llvm::Instruction &ins) {
    _type = Type::Unknown;
    llvm::raw_string_ostream rso(_llvm_name);
    ins.print(rso);
    for (unsigned int i = 0; i < ins.getNumOperands(); ++i) {
        _params.emplace_back(Value(ins.getOperand(i), 1));
    }
}

const Value &Instruction::getParam(size_t _index) const {
    assert(_index < _params.size());
    return _params[_index];
}

#endif //CODESIMILARITY_INSTRUCTION_HPP
