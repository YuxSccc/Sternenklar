//
// Created by meternal on 4/20/20.
//

#ifndef CODESIMILARITY_ADMINCONFIG_H
#define CODESIMILARITY_ADMINCONFIG_H

#include "CodeSimilariry.h"
#include "Admin/AdminConfig.h"

namespace ster {
    class AdminConfig {
    private:
        bool _is_multithreading = false;
        uint32_t _thread_number = 4;
        vector <string> _filename_list;

    public:
        bool is_multithreading() const { return _is_multithreading; }

        int32_t get_thread_number() const { return _thread_number; }

        void set_thread_number(uint32_t _tn) {
            _is_multithreading = true;
            _thread_number = _tn;
        }

        void add_filename_from_folder(const string &_folder);

        const vector <string> &get_filename_list_refrence() const { return _filename_list; }
    };
}

#endif //CODESIMILARITY_ADMINCONFIG_H
