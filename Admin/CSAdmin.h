//
// Created by meternal on 4/14/20.
//

#ifndef CODESIMILARITY_CSADMIN_H
#define CODESIMILARITY_CSADMIN_H

#include "CodeSimilariry.h"
#include "Admin/AdminConfig.h"

namespace ster {
    class CSAdmin;

    REGISTER_PTR(CSAdmin);

    class CSAdmin {
    private:
        class SimilarityResult {
        private:
            // TODO: save code instance
            string _filename_A;
            string _filename_B;
            double _similarity;
        public:
            SimilarityResult(string _lhs, string _rhs, double _sim) :
                    _filename_A(std::move(_lhs)), _filename_B(std::move(_rhs)), _similarity(_sim) {
            };

            bool operator<(const SimilarityResult &_rhs) const {
                return _similarity < _rhs._similarity;
            }

            friend std::ostream &operator<<(std::ostream &os, const SimilarityResult &_sim) {
                os << "{ CodeA:" << get_filename_from_dir(_sim._filename_A) << "}  ";
                os << "{ CodeB:" << get_filename_from_dir(_sim._filename_B) << "}  Similarity = "
                   << _sim._similarity * 100 << "%.";
                return os;
            }

            double get_similarity() const { return _similarity; }
        };

    private:
        AdminConfig _config;

        vector<string> _compile_fail_filename;
    public:
        void load_config(AdminConfig _config);

        void run();

        ~CSAdmin() {
            _config.remove_temp_IR_folder();
        }
    };
}

#endif //CODESIMILARITY_CSADMIN_H
