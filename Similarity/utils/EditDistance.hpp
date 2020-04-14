//
// Created by Meternal on 2020/4/13.
//

#ifndef CODESIMILARITY_EDITDISTANCE_HPP
#define CODESIMILARITY_EDITDISTANCE_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Node.hpp"
#include "Similarity/utils/KuhnMunkres.hpp"

#include <map>
#include <algorithm>

class EditDistance {
    // strengthen by each Ins/param
    static constexpr double normalize_dp_param = 0.02;
    static constexpr double normalize_KMnode = 0.02;
private:
    static const std::map<Instruction::Type, std::map<Instruction::Type, double>> _typeMatric;
    static const std::map<Value::Type, std::map<Value::Type, double>> _valueMatric;
    static const std::map<Value::Type, double> _delCost;

private:
    double _TypeDistance(const Instruction &, const Instruction &) const;

    double _ParamDistance(const Instruction &, const Instruction &) const;

public:
    EditDistance() = default;

    double getDistance(const Instruction &, const Instruction &) const;

    double getDistance(const Node &, const Node &) const;

};

double EditDistance::getDistance(const Instruction &_lhs, const Instruction &_rhs) const {
    const double _OpeSimilarity = _TypeDistance(_lhs, _rhs);
    const double _ParamSimilarity = _ParamDistance(_lhs, _rhs);
    // TODO: should add weight compensation to _ParamSimilarity
    return _OpeSimilarity * _ParamSimilarity;
}

double EditDistance::getDistance(const Node &_lhs, const Node &_rhs) const {
    vector<double> _weight;
    vector<std::pair<int, int>> _edge;
    for (int i = 0; i < _lhs.size(); ++i) {
        for (int j = 0; j < _rhs.size(); ++j) {
            _edge.emplace_back(std::make_pair(i, j));
            _weight.push_back(getDistance(*_lhs.getIns(i), *_rhs.getIns(j)));
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
        const auto iteratorA = _typeMatric.find(typeA);
        assert(iteratorA != _typeMatric.end());
        const auto iteratorB = iteratorA->second.find(typeB);
        assert(iteratorB != iteratorA->second.end());
        return iteratorB->second;
    }
    // both call instruction, need to check called function
    // TODO: more detailed detection (e.g push_back and emplace_back)
    // TODO: declare getCallfunction()
    return _lhs.getParam(1) == _rhs.getParam(1);
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
    uint32_t _delCount = (uint32_t) abs((int) _lhsParams.size() - (int) _rhsParams.size()) + 1;
    // Gen dp[l.size][r.size][delCount]

    const double dp_INIT = 1e20;
    _dp.resize(_lhsParams.size() + 1,
               vector<vector<double>>(_rhsParams.size() + 1,
                                      vector<double>(_delCount, dp_INIT)));
    _dp[0][0][_delCount] = 0;
    for (int i = 0; i <= _lhsParams.size(); ++i) {
        for (int j = 0; j <= _rhsParams.size(); ++j) {
            for (int k = 0; k < _delCount; ++k) {
                if (k && j < _rhsParams.size()) {
                    // Delete ele
                    auto iter = _delCost.find(_rhsParams[j].getType());
                    assert(iter != _delCost.end());
                    _dp[i][j + 1][k - 1] = std::min(_dp[i][j + 1][k - 1], _dp[i][j][k] + iter->second);
                }
                // edit ele
                if (i < _lhsParams.size() && j < _rhsParams.size()) {
                    auto iterA = _valueMatric.find(_lhsParams[i].getType());
                    assert(iterA != _valueMatric.end());
                    auto iterB = iterA->second.find(_rhsParams[j].getType());
                    assert(iterB != iterA->second.end());
                    _dp[i + 1][j + 1][k] = std::min(_dp[i + 1][j + 1][k], _dp[i][j][k] + iterB->second);
                }
            }
        }
    }
    // TODO: choose better normalize function
    return _dp[_lhsParams.size()][_rhsParams.size()][0] / _rhsParams.size() *
           (1 + normalize_dp_param * _rhsParams.size());
}

const std::map<Instruction::Type, std::map<Instruction::Type, double>> EditDistance::_typeMatric = {
        // TODO: matric init...
        // TODO: handle undefined Type
};

const std::map<Value::Type, std::map<Value::Type, double>> EditDistance::_valueMatric = {
        // TODO: matric init...
};

const std::map<Value::Type, double> EditDistance::_delCost = {
        // TODO: matric init...
};

#endif //CODESIMILARITY_EDITDISTANCE_HPP
