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
        static constexpr double normalize_dp_param = 0.02;
        static constexpr double normalize_KMnode = 0.02;
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
        double _ParamSimilarity = _ParamDistance(_lhs, _rhs);
        // TODO: should add weight compensation to _ParamSimilarity
        return _OpeSimilarity * _ParamSimilarity;
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
        size_t _maxInsCount = std::max(_lhs.size(), _rhs.size());
        // TODO: choose better normalize function
        return _km.Match(_lhs.size(), _rhs.size(), _edge, _weight)
               / _maxInsCount * (normalize_KMnode * _maxInsCount + 1);
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
        return _lhs.getParam(0) == _rhs.getParam(0);
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
        return _dp[_lhsParams.size()][_rhsParams.size()][0] / (0.01 + (_rhsParams.size() *
                                                                       (1 + normalize_dp_param * _rhsParams.size())));
    }

    constexpr double EditDistance::_DeleteValueCost[magic_enum::enum_count<Value::Type>()] = {
            0.20,   // VoidTy
            0.20,   // FloatTy
            0.20,   // IntegerTy
            0.20,   // FunctionTy
            0.20,   // StructTy
            0.20,   // ArrayTy
            0.20,   // PointerTy
            0.20,   // VectorTy
            0.20,   // OthersTy
            0.00    // Unknown
    };
}
#endif //CODESIMILARITY_EDITDISTANCE_HPP
