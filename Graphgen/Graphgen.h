//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_GRAPHGEN_H
#define CODESIMILARITY_GRAPHGEN_H

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

#include "Graphgen/GenFilter/GenFilter.h"

class Graphgen {
private:
    vector<GenFilterPtr> _Filters;

public:
    int addFliter(GenFilterPtr ptr);

    int removeFliter(int index);

    GraphPtr gen(string filename) const;
};

#endif //CODESIMILARITY_GRAPHGEN_H
