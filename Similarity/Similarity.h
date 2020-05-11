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
        double getSimilarity(GraphPtr &_lhs, GraphPtr &_rhs, vector<std::pair<int, double>> &_left_match);
    };
}

#endif //CODESIMILARITY_SIMILARITY_H
