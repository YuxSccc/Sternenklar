//
// Created by meternal on 4/4/20.
//

#ifndef CODESIMILARITY_VALUE_HPP
#define CODESIMILARITY_VALUE_HPP

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace ster {
    class Value {
    public:
        enum class Type {
            VoidTy = 0,
            FloatTy,
            IntegerTy,
            FunctionTy,
            StructTy,
            ArrayTy,
            PointerTy,
            VectorTy,
            OthersTy, // metadata, label, mmx
            Unknown
        };

    private:
        Value::Type _type;
        type_t _Typeid;

    private:

        int _weight;

        std::string _name;
        bool _hasName;

    public:
        inline bool operator==(const Value &_rhs) const;

    public:
        inline Value(llvm::Value *val, int weight);

        inline const std::string &getName() const { return _name; }

        inline Type getType() const { return _type; }

        type_t getTypeid() const { return _Typeid; }

    };

    Value::Value(llvm::Value *val, int weight) {
        _weight = weight;
        llvm::Type *_Ty = val->getType();
        _Typeid = _Ty->getTypeID();
        if (_Ty->isVoidTy()) { _type = Type::VoidTy; }
        else if (_Ty->isIntegerTy()) { _type = Type::IntegerTy; }
        else if (_Ty->isFunctionTy()) { _type = Type::FunctionTy; }
        else if (_Ty->isPointerTy()) { _type = Type::PointerTy; }
        else if (_Ty->isStructTy()) { _type = Type::StructTy; }
        else if (_Ty->isArrayTy()) { _type = Type::ArrayTy; }
        else if (_Ty->isVectorTy()) { _type = Type::VectorTy; }
        else if (_Ty->isFloatingPointTy()) { _type = Type::FloatTy; }
        else if (_Ty->isLabelTy() || _Ty->isMetadataTy() || _Ty->isTokenTy() ||
                 _Ty->isX86_MMXTy()) { _type = Type::OthersTy; }
        else { _type = Type::Unknown; }
        _hasName = val->hasName();
        if (val->hasName()) _name = val->getName().data();
        assert(_type != Type::Unknown);
    }

    bool Value::operator==(const Value &_rhs) const {
        return _type == _rhs._type && _name == _rhs._name;
    }

}
#endif //CODESIMILARITY_VALUE_HPP
