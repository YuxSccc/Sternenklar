//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_EDITDISTANCEMETHOD_HPP
#define CODESIMILARITY_EDITDISTANCEMETHOD_HPP

#include "Similarity/TextSimilarity/TextSimilarity.hpp"

#include "Similarity/utils/EditDistance.hpp"

class EditDistanceMethod : public TextSimilarity {
    double Calculate(const Node &_lhs, const Node &_rhs) override {
        EditDistance _editDistance;
        return _editDistance.getDistance(_lhs, _rhs);
    }
};


#endif //CODESIMILARITY_EDITDISTANCEMETHOD_HPP
