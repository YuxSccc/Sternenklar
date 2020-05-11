//
// Created by meternal on 4/20/20.
//

#ifndef CODESIMILARITY_ADMINCONFIG_H
#define CODESIMILARITY_ADMINCONFIG_H

#include "CodeSimilariry.h"

#include <filesystem>

namespace ster {
    class AdminConfig {
    private:
        bool _is_multithreading = false;
        uint32_t _thread_number = 4;

        string _compiler_path;
        vector <std::filesystem::path> _source_code_list;
        vector <std::pair<std::filesystem::path, std::filesystem::path>> _IR_list;
        std::filesystem::path _IR_folder_path;

    public:
        AdminConfig() {
            create_if_not_exist_IR_folder();
        }

    public:
        bool is_multithreading() const { return _is_multithreading; }

        void set_compiler_path(string _temp_path) { _compiler_path = std::move(_temp_path); }

        void add_IR_list_element(std::filesystem::path _temp_path, std::filesystem::path _source_code_path) {
            _IR_list.push_back(std::move(std::make_pair(_temp_path, _source_code_path)));
        }

        std::filesystem::path get_IR_folder_path() const { return _IR_folder_path; }

        int32_t get_thread_number() const { return _thread_number; }

        string get_compiler_path() const { return _compiler_path; }

        void set_thread_number(uint32_t _tn) {
            _is_multithreading = true;
            _thread_number = _tn;
        }

        void add_source_code_from_folder(const string &_folder);

        void create_if_not_exist_IR_folder();

        void remove_temp_IR_folder();

        const vector <std::filesystem::path> &get_source_code_list_refrence() const { return _source_code_list; }

        const vector <std::pair<std::filesystem::path, std::filesystem::path>> &
        get_IR_list_refrence() const { return _IR_list; }
    };
}

#endif //CODESIMILARITY_ADMINCONFIG_H
