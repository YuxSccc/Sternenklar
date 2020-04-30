//
// Created by meternal on 4/7/20.
//

#ifndef CODESIMILARITY_GRAPHSIMILARITY_HPP
#define CODESIMILARITY_GRAPHSIMILARITY_HPP

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

#include "Eigen/Dense"

class GraphSimilarity {
public:
    using matrix = Eigen::MatrixXd;
    using matrixPtr = std::shared_ptr<matrix>;

    virtual matrix match() = 0;
};


#endif //CODESIMILARITY_GRAPHSIMILARITY_HPP
