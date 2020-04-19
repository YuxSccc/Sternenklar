//
// Created by meternal on 4/19/20.
//

#ifndef CODESIMILARITY_DEBUGINSTRUCTIONFILTER_H
#define CODESIMILARITY_DEBUGINSTRUCTIONFILTER_H

#include "CodeSimilariry.h"
#include "Graphgen/GenFilter/GenFilter.h"

namespace ster {
    class DebugInstructionFilter;

    REGISTER_PTR(DebugInstructionFilter);

    class DebugInstructionFilter : public GenFilter {
    public:
        int Filter(vector<Instruction *> &fil) override;
    };
}

#endif //CODESIMILARITY_DEBUGINSTRUCTIONFILTER_H
