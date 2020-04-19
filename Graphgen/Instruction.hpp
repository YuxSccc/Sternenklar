//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_INSTRUCTION_HPP
#define CODESIMILARITY_INSTRUCTION_HPP

#include <llvm/IR/Instructions.h>
#include "CodeSimilariry.h"
#include "Graphgen/Value.hpp"

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instruction.def"
#include "llvm/IR/InstrTypes.h"

#define INSTRUCTION_TYPE_COUNT

namespace ster {
    class Instruction {
    public:
        enum class Type {
            TerminatorInst = 0,

            AddOpeInst,
            SubOpeInst,
            MulOpeInst,
            DivOpeInst,
            RemOpeInst,

            LogicalOpeInst,

            MemoryInst,

            ConvertInst,

            CallInst,
            CmpInst,
            BitOpeInst,
            OtherInst,

            Unknown,
        };

        enum class TopType {
            TerminatorInst = 0,
            MathInst,
            LogicalOpeInst,
            MemoryInst,
            ConvertInst,
            OtherInst,
            Unknown
        };

    private:
        Type _type;
        vector<Value> _params;
        std::string _llvm_name;
        uint32_t _Opcode;

    private:
        inline void _classifyInstructionType(const llvm::Instruction &_ins);

    public:
        inline explicit Instruction(const llvm::Instruction &ins);

        ~Instruction() = default;

    public:
        Type getType() const { return _type; };

        size_t paramSize() const { return _params.size(); }

        uint32_t getOpcode() const { return _Opcode; }

        inline const Value &getParam(size_t _index) const;

        inline TopType getTopType() const;

        vector<Value> getParamsCopy() const {
            return vector<Value>(_params);
        }

    };

//----------------------------------------------------------------------------------------------------

    Instruction::Instruction(const llvm::Instruction &_ins) {
        _type = Type::Unknown;
        llvm::raw_string_ostream rso(_llvm_name);
        _ins.print(rso);
        for (unsigned int i = 0; i < _ins.getNumOperands(); ++i) {
            _params.emplace_back(Value(_ins.getOperand(i), 1));
        }
        _Opcode = _ins.getOpcode();
        _classifyInstructionType(_ins);
        assert(_type != Type::Unknown);
    }

    const Value &Instruction::getParam(size_t _index) const {
        assert(_index < _params.size());
        return _params[_index];
    }

    void Instruction::_classifyInstructionType(const llvm::Instruction &_ins) {
        switch (_Opcode) {
            // Terminators
            case llvm::Instruction::Ret:
            case llvm::Instruction::Br:
            case llvm::Instruction::Switch:
            case llvm::Instruction::IndirectBr:
            case llvm::Instruction::Invoke:
            case llvm::Instruction::Resume:
            case llvm::Instruction::Unreachable:
            case llvm::Instruction::CleanupRet:
            case llvm::Instruction::CatchRet:
            case llvm::Instruction::CatchPad:
            case llvm::Instruction::CatchSwitch:
                _type = Type::TerminatorInst;
                break;

                // Standard unary operators...
            case llvm::Instruction::FNeg:
                _type = Type::LogicalOpeInst;
                break;

                // Standard binary operators...
            case llvm::Instruction::Add:
            case llvm::Instruction::FAdd:
                _type = Type::AddOpeInst;
                break;

            case llvm::Instruction::Sub:
            case llvm::Instruction::FSub:
                _type = Type::SubOpeInst;
                break;

            case llvm::Instruction::Mul:
            case llvm::Instruction::FMul:
                _type = Type::MulOpeInst;
                break;

            case llvm::Instruction::UDiv:
            case llvm::Instruction::SDiv:
            case llvm::Instruction::FDiv:
                _type = Type::DivOpeInst;
                break;
            case llvm::Instruction::URem:
            case llvm::Instruction::SRem:
            case llvm::Instruction::FRem:
                _type = Type::RemOpeInst;
                break;

                // Logical operators...
            case llvm::Instruction::And:
            case llvm::Instruction::Or :
            case llvm::Instruction::Xor:
                _type = Type::LogicalOpeInst;
                break;

                // Memory instructions...
            case llvm::Instruction::Alloca:
            case llvm::Instruction::Load:
            case llvm::Instruction::Store:
            case llvm::Instruction::AtomicCmpXchg:
            case llvm::Instruction::AtomicRMW:
            case llvm::Instruction::Fence:
            case llvm::Instruction::GetElementPtr:
                _type = Type::MemoryInst;
                break;

                // Convert instructions...
            case llvm::Instruction::Trunc:
            case llvm::Instruction::ZExt:
            case llvm::Instruction::SExt:
            case llvm::Instruction::FPTrunc:
            case llvm::Instruction::FPExt:
            case llvm::Instruction::FPToUI:
            case llvm::Instruction::FPToSI:
            case llvm::Instruction::UIToFP:
            case llvm::Instruction::SIToFP:
            case llvm::Instruction::IntToPtr:
            case llvm::Instruction::PtrToInt:
            case llvm::Instruction::BitCast:
            case llvm::Instruction::AddrSpaceCast:
                _type = Type::ConvertInst;
                break;

                // Other instructions...
            case llvm::Instruction::ICmp:
            case llvm::Instruction::FCmp:
                _type = Type::CmpInst;
                break;

            case llvm::Instruction::PHI:
            case llvm::Instruction::Select:
                _type = Type::OtherInst;

            case llvm::Instruction::Call:
                _type = Type::CallInst;
                break;

            case llvm::Instruction::Shl:
            case llvm::Instruction::LShr:
            case llvm::Instruction::AShr:
                _type = Type::BitOpeInst;
                break;

                // Weak feature Instruction
            case llvm::Instruction::VAArg:
            case llvm::Instruction::ExtractElement:
            case llvm::Instruction::InsertElement:
            case llvm::Instruction::ShuffleVector:
            case llvm::Instruction::ExtractValue:
            case llvm::Instruction::InsertValue:
            case llvm::Instruction::LandingPad:
            case llvm::Instruction::CleanupPad:
                _type = Type::OtherInst;
                break;

            default:
                _type = Type::Unknown;
        }
    }

    Instruction::TopType Instruction::getTopType() const {
        switch (_type) {
            case Type::TerminatorInst:
                return TopType::TerminatorInst;
            case Type::AddOpeInst:
            case Type::SubOpeInst:
            case Type::MulOpeInst:
            case Type::DivOpeInst:
            case Type::RemOpeInst:
                return TopType::MathInst;
            case Type::LogicalOpeInst:
                return TopType::LogicalOpeInst;
            case Type::MemoryInst:
                return TopType::MemoryInst;
            case Type::ConvertInst:
                return TopType::ConvertInst;
            case Type::CallInst:
            case Type::CmpInst:
            case Type::BitOpeInst:
            case Type::OtherInst:
                return TopType::OtherInst;
            default:
                return TopType::Unknown;
        }
    }
}
#endif //CODESIMILARITY_INSTRUCTION_HPP
