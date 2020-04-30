//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_EDITDISTANCEMETHOD_HPP
#define CODESIMILARITY_EDITDISTANCEMETHOD_HPP

#include "Similarity/TextSimilarity/TextSimilarity.hpp"

#include "Similarity/utils/EditDistance.hpp"

namespace ster {
    class EditDistanceMethod : public TextSimilarity {
        double Calculate(const Node &_lhs, const Node &_rhs) override {
            EditDistance _editDistance;
            return _editDistance.getDistance(_lhs, _rhs);
        }

        matrix Calculate(const GraphPtr _lhs, const GraphPtr _rhs) override {
            matrixPtr _initMat(new matrix(_lhs->size(), _rhs->size()));
            for (size_t i = 0; i < _lhs->size(); ++i) {
                for (size_t j = 0; j < _rhs->size(); ++j) {
                    (*_initMat)(i, j) = Calculate(*_lhs->getNodePtrAt(i), *_rhs->getNodePtrAt(j));
                }
            }
            return matrix(*_initMat);
        }

    };
}

#endif //CODESIMILARITY_EDITDISTANCEMETHOD_HPP
