//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_EDITDISTANCEMETHOD_HPP
#define CODESIMILARITY_EDITDISTANCEMETHOD_HPP

#include "Similarity/TextSimilarity/TextSimilarity.hpp"

#include "Similarity/utils/KuhnMunkres.hpp"

class EditDistanceMethod : public TextSimilarity {
    double Calculate(const Node &, const Node &) override {

    }
};


#endif //CODESIMILARITY_EDITDISTANCEMETHOD_HPP
