//
// Created by meternal on 4/18/20.
//

#ifndef CODESIMILARITY_TYPESIMILARITY_H
#define CODESIMILARITY_TYPESIMILARITY_H

#include "Graphgen/Instruction.hpp"
#include "utils/magic_enum.hpp"

namespace ster {
    class TypeSimilarity {
        static constexpr double SAME = 1.00;
        static constexpr double INSTRUCTION_SAME_TYPE_SIM = 0.90;
        static constexpr double DIFFERENT = 0.00;

        static constexpr double VALUE_SAME_TYPE_SIM = 0.90;
        static constexpr double VALUE_INT_AND_FLOAT_SIM = 0.40;


        static constexpr size_t Instruction_Type_Count = magic_enum::enum_count<Instruction::Type>();
        static constexpr size_t Instruction_TopType_Count = magic_enum::enum_count<Instruction::TopType>();
        static constexpr size_t Value_Type_Count = magic_enum::enum_count<Value::Type>();

        static const double _instructionSameTTyInternalSimilarity[Instruction_TopType_Count];

        static bool _InstructionTopTypeHasSubType(Instruction::TopType _chk);

    public:
        static double TypeSimilarityOfTwoInstruction(const Instruction &_lhs, const Instruction &_rhs);

        static double TypeSimilarityOfTwoValue(const Value &_lhs, const Value &_rhs);
    };
}


#endif //CODESIMILARITY_TYPESIMILARITY_H
