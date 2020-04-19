//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_TEXTSIMILARITY_HPP
#define CODESIMILARITY_TEXTSIMILARITY_HPP

#include "CodeSimilariry.h"

#include "Graphgen/Graph.hpp"
#include "Graphgen/Node.hpp"

#include "Eigen/Dense"

namespace ster {
    class TextSimilarity {
    public:
        using matrix = Eigen::MatrixXd;
        using matrixPtr = std::shared_ptr<matrix>;

        virtual matrix Calculate(const GraphPtr, const GraphPtr) = 0;

        virtual double Calculate(const Node &, const Node &) = 0;
    };
}

#endif //CODESIMILARITY_TEXTSIMILARITY_HPP
