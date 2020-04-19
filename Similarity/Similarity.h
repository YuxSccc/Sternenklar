//
// Created by meternal on 4/19/20.
//

#ifndef CODESIMILARITY_SIMILARITY_H
#define CODESIMILARITY_SIMILARITY_H

#include "CodeSimilariry.h"
#include "Graphgen/Graph.hpp"

#include "Similarity/GraphSimilarity/GSSM.h"
#include "Similarity/TextSimilarity/EditDistanceMethod.hpp"
#include "Similarity/utils/KuhnMunkres.hpp"

namespace ster {
    class Similarity {
    public:
        double getSimilarity(GraphPtr &_lhs, GraphPtr &_rhs) {
            shared_ptr<GraphSimilarity> GS(new GSSM(_lhs, _rhs));
            shared_ptr<TextSimilarity> TS(new EditDistanceMethod());

            int tempa = clock();
            auto GraphMat = GS->match();
            int tempb = clock();
            auto TextMat = TS->Calculate(_lhs, _rhs);
            cout << (1.0 * tempb - tempa) / CLOCKS_PER_SEC * 1000 << " "
                 << (1.0 * clock() - tempb) / CLOCKS_PER_SEC * 1000 << endl;

            GSSM::matrix _tans(_lhs->size(), _rhs->size());

            for (size_t i = 0; i < _lhs->size(); ++i) {
                for (size_t j = 0; j < _rhs->size(); ++j) {
                    if (GraphMat(i, j) <= 0.001) GraphMat(i, j) = 0;
                    _tans(i, j) = (1 + GraphMat(i, j) / 5) * TextMat(i, j);
                }
            }

            cout << "GRAPHMAT" << endl;
            cout << GraphMat << endl;
            cout << "TXGRAPHMAT" << endl;
            cout << TextMat << endl;

            vector<std::pair<int, int>> _edges;
            vector<double> weight;
            vector<int> leftmatch;

            for (size_t i = 0; i < _lhs->size(); ++i) {
                for (size_t j = 0; j < _rhs->size(); ++j) {
                    _edges.emplace_back(std::make_pair(i, j));
                    weight.push_back(_tans(i, j));
                }
            }
            int matchNodeCount = 0;
            KuhnMunkres t;
            double match_val = t.Match(_lhs->size(), _rhs->size(), _edges, weight, leftmatch);
            for (size_t i = 0; i < leftmatch.size(); ++i) {
                if (leftmatch[i] == -1) continue;
                matchNodeCount++;
                cout << "Node[" << i << "] in CodeA Match Node[" << leftmatch[i] << "] in CodeB" << endl;
            }
            cout << matchNodeCount << "Nodes matched" << endl;
            cout << "matchval / matchcount = " << match_val / matchNodeCount * 100 << "%" << endl;
            return match_val;
        }
    };
}

#endif //CODESIMILARITY_SIMILARITY_H
