//
// Created by meternal on 4/19/20.
//

#include "CodeSimilariry.h"
#include "DebugInstructionFilter.h"

namespace ster {
    int DebugInstructionFilter::Filter(vector<Instruction *> &fil) {
        vector<Instruction *> _CleanIns;
        for (const auto &item : fil) {
            bool _isok = true;
            for (int i = 0; i < (int) item->paramSize(); ++i) {
                if (item->getParam(i).getName() == "llvm.dbg.value") {
                    _isok = false;
                    break;
                }
            }
            if (_isok) {
                _CleanIns.push_back(item);
            }
        }
        fil.swap(_CleanIns);
        return 0;
    }
}