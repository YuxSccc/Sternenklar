//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_CODESIMILARIRY_H
#define CODESIMILARITY_CODESIMILARIRY_H

#include <vector>
#include <string>
#include <memory>
#include <cassert>

using std::vector;
using std::string;

using std::shared_ptr;
using std::unique_ptr;

#define REGISTER_PTR(Ty) \
    using Ty##Ptr = shared_ptr<Ty>

#endif //CODESIMILARITY_CODESIMILARIRY_H
