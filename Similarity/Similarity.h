//
// Created by meternal on 4/19/20.
//

#ifndef CODESIMILARITY_SIMILARITY_H
#define CODESIMILARITY_SIMILARITY_H

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

#include "Similarity/GraphSimilarity/GSSM.h"
#include "Similarity/TextSimilarity/EditDistanceMethod.hpp"
#include "Similarity/utils/KuhnMunkres.hpp"

namespace ster {
    class Similarity {
    public:
        double getSimilarity(GraphPtr &_lhs, GraphPtr &_rhs) {
            shared_ptr<GraphSimilarity> GS(new GSSM(_lhs, _rhs));
            shared_ptr<TextSimilarity> TS(new EditDistanceMethod());
            auto GraphMat = GS->match();
            auto TextMat = TS->Calculate(_lhs, _rhs);
            GSSM::matrix _tans(_lhs->size(), _rhs->size());
            for (size_t i = 0; i < _lhs->size(); ++i) {
                for (size_t j = 0; j < _rhs->size(); ++j) {
                    _tans(i, j) = (1 + GraphMat(i, j) / 3) * TextMat(i, j);
                }
            }

            vector<std::pair<int, int>> _edges;
            vector<double> weight;
            vector<int> leftmatch;

            for (size_t i = 0; i < _lhs->size(); ++i) {
                for (size_t j = 0; j < _rhs->size(); ++j) {
                    _edges.emplace_back(std::make_pair(i, j));
                    weight.push_back(_tans(i, j));
                }
            }

            KuhnMunkres t;
            return t.Match(_lhs->size(), _rhs->size(), _edges, weight, leftmatch);
        }
    };
}

#endif //CODESIMILARITY_SIMILARITY_H
