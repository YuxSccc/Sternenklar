//
// Created by meternal on 4/19/20.
//

#include "Similarity.h"

namespace ster {
    double Similarity::getSimilarity(GraphPtr &_lhs, GraphPtr &_rhs) {
        if (_lhs->empty() || _rhs->empty()) {
            LOG(WARNING) << _lhs->get_source_filename() << " " << _rhs->get_source_filename() << " has empty Code\n";
            return 0;
        }
        shared_ptr<GraphSimilarity> GS(new GSSM(_lhs, _rhs));
        shared_ptr<TextSimilarity> TS(new EditDistanceMethod());

        auto GraphMat = GS->match();
        auto TextMat = TS->Calculate(_lhs, _rhs);

        GSSM::matrix _tans(_lhs->size(), _rhs->size());

        for (size_t i = 0; i < _lhs->size(); ++i) {
            for (size_t j = 0; j < _rhs->size(); ++j) {
                if (GraphMat(i, j) <= 0.001) GraphMat(i, j) = 0;
                _tans(i, j) = (1 + GraphMat(i, j) / 5) * TextMat(i, j);
            }
        }

        vector<std::pair<int, int>> _edges;
        vector<double> weight;
        vector<int> leftmatch;

        for (size_t i = 0; i < _lhs->size(); ++i) {
            for (size_t j = 0; j < _rhs->size(); ++j) {
                _edges.emplace_back(std::make_pair(i, j));
                weight.push_back(_tans(i, j));
            }
        }
        KuhnMunkres t;
        double match_val = t.Match(_lhs->size(), _rhs->size(), _edges, weight, leftmatch);

        return match_val / (double) ((_lhs->size() + _rhs->size()) / 2);
    }
}