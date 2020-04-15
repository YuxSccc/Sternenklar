//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_TEXTSIMILARITY_HPP
#define CODESIMILARITY_TEXTSIMILARITY_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Node.hpp"

class TextSimilarity {
public:
    virtual double Calculate(const Node &, const Node &) = 0;
};


#endif //CODESIMILARITY_TEXTSIMILARITY_HPP
