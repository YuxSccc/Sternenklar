#include <iostream>

#include "Graphgen/Graphgen.h"
#include "Similarity/GraphSimilarity/GSSM.h"
#include "Similarity/TextSimilarity/TextSimilarity.hpp"
#include "Similarity/TextSimilarity/EditDistanceMethod.hpp"

#include "Similarity/Similarity.h"

using namespace std;
using namespace ster;

int main() {
    std::cout << "Hello, World!" << std::endl;
    Graphgen generator;
    GraphPtr ptr1 = generator.gen("/home/meternal/paper/test0.ll");
    GraphPtr ptr2 = generator.gen("/home/meternal/paper/test1.ll");
    assert(ptr1 && ptr2);
    /*
    shared_ptr<GraphSimilarity> GS(new GSSM(ptr1, ptr2));
    shared_ptr<TextSimilarity> TS(new EditDistanceMethod());
    assert(GS);
    GSSM::matrix mat = GS->match();
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            cout << mat(i, j) << " ";
        }
        cout << endl;
    }
    */
    Similarity sim;
    cout << sim.getSimilarity(ptr1, ptr2) << endl;

    return 0;
}