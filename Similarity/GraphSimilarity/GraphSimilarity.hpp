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
    virtual Eigen::Matrix2d match() = 0;
};


#endif //CODESIMILARITY_GRAPHSIMILARITY_HPP
