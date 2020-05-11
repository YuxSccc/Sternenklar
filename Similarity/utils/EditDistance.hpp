//
// Created by Meternal on 2020/4/13.
//

#ifndef CODESIMILARITY_EDITDISTANCE_HPP
#define CODESIMILARITY_EDITDISTANCE_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Node.hpp"
#include "Similarity/utils/KuhnMunkres.hpp"
#include "Similarity/TextSimilarity/TypeSimilarity.h"
#include "utils/magic_enum.hpp"

#include <map>
#include <algorithm>

namespace ster {
    class EditDistance {
        // strengthen by each Ins/param
        static constexpr double normalize_dp_param = 0.01;
        static constexpr double normalize_KMnode = 0.02;
        static constexpr double similarity_between_instruction_limit = 0.40;

    private:
        static const double _DeleteValueCost[magic_enum::enum_count<Value::Type>()];

    private:
        inline double _TypeDistance(const Instruction &, const Instruction &) const;

        inline double _ParamDistance(const Instruction &, const Instruction &) const;

    public:
        EditDistance() = default;

        inline double getDistance(const Instruction &, const Instruction &) const;

        inline double getDistance(const Node &, const Node &) const;

    };

    double EditDistance::getDistance(const Instruction &_lhs, const Instruction &_rhs) const {
        double _OpeSimilarity = _TypeDistance(_lhs, _rhs);
        double _ParamDis = _ParamDistance(_lhs, _rhs);
        // TODO: should add weight compensation to _ParamSimilarity
        double _ParamSimilarity =
                1.0 - sqrt(_ParamDis / std::max(std::max(_lhs.getParamSize(), _rhs.getParamSize()), (size_t) 1));
        double _InsSimilarity = _OpeSimilarity * _ParamSimilarity;
        return _InsSimilarity < similarity_between_instruction_limit ? 0 : _InsSimilarity;
    }

    double EditDistance::getDistance(const Node &_lhs, const Node &_rhs) const {
        vector<double> _weight;
        vector<std::pair<int, int>> _edge;
        for (size_t i = 0; i < _lhs.size(); ++i) {
            for (size_t j = 0; j < _rhs.size(); ++j) {
                _edge.emplace_back(std::make_pair(i, j));
                _weight.push_back(getDistance(_lhs.getIns(i), _rhs.getIns(j)));
            }
        }
        KuhnMunkres _km;
        double _avgCount = (_lhs.size() + _rhs.size()) / 2.0;
        // TODO: choose better normalize function
        return _km.Match(_lhs.size(), _rhs.size(), _edge, _weight) / _avgCount;
    }

    double EditDistance::_TypeDistance(const Instruction &_lhs, const Instruction &_rhs) const {
        const Instruction::Type typeA = _lhs.getType();
        const Instruction::Type typeB = _rhs.getType();
        if (typeA != Instruction::Type::CallInst || typeB != Instruction::Type::CallInst) {
            return TypeSimilarity::TypeSimilarityOfTwoInstruction(_lhs, _rhs);
        }
        // both call instruction, need to check called function
        // TODO: more detailed detection (e.g push_back and emplace_back)
        // TODO: declare getCallfunction()
        return _lhs.getCalledFunction() == _rhs.getCalledFunction();
    }

    double EditDistance::_ParamDistance(const Instruction &_lhs, const Instruction &_rhs) const {
        // TODO: performance problem (need to save pointer in params)
        vector<Value> _lhsParams = _lhs.getParamsCopy();
        vector<Value> _rhsParams = _rhs.getParamsCopy();
        auto _tempSortFunc = [](Value &_lhsL, Value &_rhsL) {
            return _lhsL.getName() < _rhsL.getName();
        };
        std::sort(_lhsParams.begin(), _lhsParams.end(), _tempSortFunc);
        std::sort(_rhsParams.begin(), _rhsParams.end(), _tempSortFunc);
        if (_lhsParams.size() > _rhsParams.size()) _lhsParams.swap(_rhsParams);

        vector<vector<vector<double>>> _dp;
        uint32_t _delCount = (uint32_t) abs((int) _lhsParams.size() - (int) _rhsParams.size());
        // Gen dp[l.size][r.size][delCount]

        const double dp_INIT = 1e20;
        _dp.resize(_lhsParams.size() + 1,
                   vector<vector<double>>(_rhsParams.size() + 1,
                                          vector<double>(_delCount + 1, dp_INIT)));
        _dp[0][0][_delCount] = 0;
        for (size_t i = 0; i <= _lhsParams.size(); ++i) {
            for (size_t j = 0; j <= _rhsParams.size(); ++j) {
                for (size_t k = 0; k <= _delCount; ++k) {
                    if (k && j < _rhsParams.size()) {
                        // Delete _rhsParams[j]
                        _dp[i][j + 1][k - 1] = std::min(_dp[i][j + 1][k - 1], _dp[i][j][k] +
                                                                              _DeleteValueCost[(type_t) _rhsParams[j].getType()]);
                    }
                    // edit ele
                    if (i < _lhsParams.size() && j < _rhsParams.size()) {
                        // TODO: bonus of same variable name
                        _dp[i + 1][j + 1][k] = std::min(_dp[i + 1][j + 1][k], _dp[i][j][k] +
                                                                              TypeSimilarity::TypeSimilarityOfTwoValue(
                                                                                      _lhsParams[i], _rhsParams[j]));
                    }
                }
            }
        }
        // TODO: choose better normalize function
        return _dp[_lhsParams.size()][_rhsParams.size()][0];
    }

    constexpr double EditDistance::_DeleteValueCost[magic_enum::enum_count<Value::Type>()] = {
            0.60,   // VoidTy
            0.60,   // FloatTy
            0.60,   // IntegerTy
            0.60,   // FunctionTy
            0.60,   // StructTy
            0.60,   // ArrayTy
            0.60,   // PointerTy
            0.60,   // VectorTy
            0.60,   // OthersTy
            0.60    // Unknown
    };
}
#endif //CODESIMILARITY_EDITDISTANCE_HPP
