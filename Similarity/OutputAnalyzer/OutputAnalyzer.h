//
// Created by Meternal on 2020/5/10.
//

#ifndef CODESIMILARITY_OUTPUTANALYZER_H
#define CODESIMILARITY_OUTPUTANALYZER_H

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

namespace ster {
    class OutputAnalyzer {
    public:
        static void
        OutputAnalyzeImage(const GraphPtr &_lhs, const GraphPtr &_rhs,
                           const vector<std::pair<int, double>> &_left_match,
                           double _similarity_value, const string &_output_dir);

        static void
        OutputAnalyzeImage(const GraphPtr &_lhs, const GraphPtr &_rhs,
                           const vector<std::pair<int, double>> &_left_match,
                           double _similarity_value);
    };
}

#endif //CODESIMILARITY_OUTPUTANALYZER_H
