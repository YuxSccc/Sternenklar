//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_GRAPHGEN_H
#define CODESIMILARITY_GRAPHGEN_H

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

#include "Graphgen/GenFilter/GenFilter.h"

namespace ster {
    class Graphgen {
    private:
        vector<GenFilterPtr> _Filters;

    public:
        int addFliter(GenFilterPtr ptr);

        int removeFliter(size_t index);

        int gen(string filename, string _source_code_filename, GraphPtr &graph) const;
    };
}
#endif //CODESIMILARITY_GRAPHGEN_H
