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
#include <functional>
#include "glog/logging.h"

using std::cout;
using std::endl;

#define REGISTER_PTR(Ty) \
    using Ty##Ptr = shared_ptr<Ty>

namespace ster {
    using std::vector;
    using std::string;

    using std::shared_ptr;
    using std::unique_ptr;

    using type_t = uint32_t;

    inline static string get_filename_from_dir(const string &_temp) {
        auto _pos = _temp.find_last_of('/');
        if (_pos == string::npos) return _temp;
        else return _temp.substr(_pos + 1);
    }
}
#endif //CODESIMILARITY_CODESIMILARIRY_H
