//
// Created by meternal on 4/20/20.
//

#include "CodeSimilariry.h"
#include "AdminConfig.h"

namespace ster {
    void AdminConfig::add_source_code_from_folder(const string &_folder) {
        for (const auto &_entry : std::filesystem::directory_iterator(_folder)) {
            if (!_entry.is_directory()) {
                string _file_extension = _entry.path().extension();
                if (_file_extension == ".cpp" ||
                    _file_extension == ".c" ||
                    _file_extension == ".cc" ||
                    _file_extension == ".cxx")
                    _source_code_list.push_back(_entry.path());
            }
        }
    }

    void AdminConfig::create_if_not_exist_IR_folder() {
        std::filesystem::path _IR_path = std::filesystem::path(std::filesystem::current_path().string() + "/tempCSLL");
        if (!std::filesystem::exists(_IR_path)) {
            std::filesystem::create_directories(_IR_path);
        }
        _IR_folder_path = _IR_path;
    }

    void AdminConfig::remove_temp_IR_folder() {
        if (std::filesystem::exists(_IR_folder_path)) {
            std::filesystem::remove_all(_IR_folder_path);
        }
    }
}