//
// Created by meternal on 4/3/20.
//

#ifndef CODESIMILARITY_CODESIMILARIRY_H
#define CODESIMILARITY_CODESIMILARIRY_H

#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <iostream>

#define REGISTER_PTR(Ty) \
    using Ty##Ptr = shared_ptr<Ty>

namespace ster {

    using std::vector;
    using std::string;

    using std::shared_ptr;
    using std::unique_ptr;

    using type_t = uint32_t;
}
#endif //CODESIMILARITY_CODESIMILARIRY_H
