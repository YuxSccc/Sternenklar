//
// Created by meternal on 4/6/20.
//

#ifndef CODESIMILARITY_SIMHASHMETHOD_HPP
#define CODESIMILARITY_SIMHASHMETHOD_HPP

#include "CodeSimilariry.h"

#include "Similarity/TextSimilarity/TextSimilarity.hpp"
#include "Similarity/utils/picosha2.hpp"
#include "Similarity/utils/EditDistance.hpp"

namespace ster {
    class SimHashMethod : public TextSimilarity {
    public:
        double Calculate(const Node &, const Node &) override {

        }
    };
}

#endif //CODESIMILARITY_SIMHASHMETHOD_HPP
