//
// Created by meternal on 4/23/20.
//

#ifndef CODESIMILARITY_RUNNERCONFIG_H
#define CODESIMILARITY_RUNNERCONFIG_H

#include "CodeSimilariry.h"

namespace ster {
    class RunnerConfig {
    private:
        static constexpr auto _deleter = [](char **_ptr) {
            char **_begin = _ptr;
            while (*_begin != nullptr) {
                delete[]*_begin;
                _begin++;
            }
            delete[]_ptr;
        };

    private:
        unique_ptr<char *[], decltype(_deleter)> _param_ptr;
        unique_ptr<char *[], decltype(_deleter)> _env_ptr;

    private:
        void _alloca_string_at_index(int _index, const string &_temp_str) {
            (_param_ptr.get())[_index] = new char[_temp_str.size() + 1];
            _temp_str.copy((_param_ptr.get())[_index], _temp_str.size(), 0);
            (_param_ptr.get())[_index][_temp_str.size()] = 0;
        }

    public:
        RunnerConfig() :
                _param_ptr(new char *[1], _deleter),
                _env_ptr(new char *[1], _deleter) {
            _param_ptr.get()[0] = nullptr;
            _env_ptr.get()[0] = nullptr;
        };

        RunnerConfig(RunnerConfig &_temp) = delete;

        RunnerConfig operator=(RunnerConfig &_temp) = delete;

    public:
        void load_cpp_compile_information(const string &_clang_path,
                                          const string &_filename,
                                          const string &_output_filename);

        void load_llvm_opt_information(const string &_opt_path,
                                       const string &_filename,
                                       const string &_output_filename);

        friend std::ostream &operator<<(std::ostream &_os, const RunnerConfig &_temp) {
            char **_begin = _temp._param_ptr.get();
            _os << "Params = ";
            while (*_begin != nullptr) {
                _os << _begin << " ";
                _begin++;
            }
            return _os;
        }

    public:
        char **get_param() { return _param_ptr.get(); }

        char **get_env() { return _env_ptr.get(); }
    };
}

#endif //CODESIMILARITY_RUNNERCONFIG_H
