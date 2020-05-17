//
// Created by meternal on 4/22/20.
//

#ifndef CODESIMILARITY_RUNNER_H
#define CODESIMILARITY_RUNNER_H

#include <CodeSimilariry.h>

namespace ster {
    class Runner {
    public:
        enum class RUN_MODE {
            CLANG_COMPILE,
            LLVM_OPT
        };
    public:
        static int run(const string &_run_path, const string &_input_file, const string &_output_file, RUN_MODE _mode);
    };
}

#endif //CODESIMILARITY_RUNNER_H
