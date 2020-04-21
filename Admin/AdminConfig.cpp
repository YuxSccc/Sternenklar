//
// Created by meternal on 4/20/20.
//

#include "CodeSimilariry.h"
#include "AdminConfig.h"

#include "utils/cxxopts.hpp"

#include <filesystem>

namespace ster {
    void AdminConfig::add_filename_from_folder(const string &_folder) {
        for (const auto &_entry : std::filesystem::directory_iterator(_folder)) {
            if (!_entry.is_directory() && _entry.path().extension() == ".ll") {
                _filename_list.push_back(_entry.path());
            }
        }
    }
}