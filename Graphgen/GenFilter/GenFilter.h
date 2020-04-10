//
// Created by meternal on 4/5/20.
//

#ifndef CODESIMILARITY_GENFILTER_H
#define CODESIMILARITY_GENFILTER_H

#include <CodeSimilariry.h>

#include <Graphgen/Instruction.hpp>

class GenFilter;

REGISTER_PTR(GenFilter);

class GenFilter {
public:
    virtual int Filter(vector<Instruction *> fil) = 0;
};


#endif //CODESIMILARITY_GENFILTER_H
