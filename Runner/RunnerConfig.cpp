//
// Created by meternal on 4/23/20.
//

#include "RunnerConfig.h"

namespace ster {
    void RunnerConfig::load_cpp_compile_information(const string &_clang_path, const string &_filename,
                                                    const string &_output_filename) {
        const int _param_size = 11;
        _param_ptr.reset(new char *[_param_size + 1]);
        _alloca_string_at_index(0, _clang_path);
        // _alloca_string_at_index(1, "-include");
        // _alloca_string_at_index(2, "bits/stdc++.h");
        _alloca_string_at_index(1, "-fno-discard-value-names");
        _alloca_string_at_index(2, "-emit-llvm");
        _alloca_string_at_index(3, "-g");
        _alloca_string_at_index(4, "-O1");
        _alloca_string_at_index(5, "-S");
        _alloca_string_at_index(6, "-Xclang");
        _alloca_string_at_index(7, "-disable-O0-optnone");
        _alloca_string_at_index(8, _filename);
        _alloca_string_at_index(9, "-o");
        _alloca_string_at_index(10, _output_filename);
        (_param_ptr.get())[11] = nullptr;
        LOG_IF(FATAL, (_param_ptr.get())[_param_size] != nullptr);
    }

    void RunnerConfig::load_llvm_opt_information(const string &_opt_path, const string &_filename,
                                                 const string &_output_filename) {
        const int _param_size = 10;
        _param_ptr.reset(new char *[_param_size + 1]);
        _alloca_string_at_index(0, _opt_path);
        _alloca_string_at_index(1, _filename);
        _alloca_string_at_index(2, "-S");
        _alloca_string_at_index(3, "-mem2reg");
        _alloca_string_at_index(4, "-dce");
        _alloca_string_at_index(5, "-constprop");
        _alloca_string_at_index(6, "-die");
        _alloca_string_at_index(7, "-dse");
        _alloca_string_at_index(8, "-o");
        _alloca_string_at_index(9, _output_filename);
        (_param_ptr.get())[10] = nullptr;
        LOG_IF(FATAL, (_param_ptr.get())[_param_size] != nullptr);
    }
}