//
// Created by meternal on 4/4/20.
//

#ifndef CODESIMILARITY_VALUE_HPP
#define CODESIMILARITY_VALUE_HPP

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

class Value {
public:
    enum class Type {
        ArrayTy,
        VectorTy,
        StructTy,
        FunctionTy,
        IntergerTy,
        PointerTy,
        Unknown
    };

private:
    Value::Type _type;
    std::string _name;
    int _weight;
    bool _hasName;
public:

    Value(llvm::Value *val, int weight);

    const std::string &getName() const { return _name; }
};

Value::Value(llvm::Value *val, int weight) {
    _weight = weight;
    llvm::Type *Ty = val->getType();
    if (Ty->isIntegerTy()) _type = Type::IntergerTy;
    else if (Ty->isFunctionTy()) _type = Type::FunctionTy;
    else if (Ty->isPointerTy()) _type = Type::PointerTy;
    else if (Ty->isStructTy()) _type = Type::StructTy;
    else if (Ty->isArrayTy()) _type = Type::ArrayTy;
    else if (Ty->isVectorTy()) _type = Type::VectorTy;
    else _type = Type::Unknown;
    _hasName = val->hasName();
    if (val->hasName()) _name = val->getName().data();
}

#endif //CODESIMILARITY_VALUE_HPP
