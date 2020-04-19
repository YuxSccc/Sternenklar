#include <iostream>

#include "Graphgen/Graphgen.h"
#include "Similarity/GraphSimilarity/GSSM.h"
#include "Similarity/TextSimilarity/TextSimilarity.hpp"
#include "Similarity/TextSimilarity/EditDistanceMethod.hpp"

#include "Similarity/Similarity.h"
#include "Graphgen/GenFilter/GenFilter.h"
#include "Graphgen/GenFilter/DebugInstructionFilter.h"

#include "ctime"

using namespace std;
using namespace ster;

int main() {
    int t = clock();
    std::cout << "Hello, World!" << std::endl;
    Graphgen generator;
    GenFilterPtr DebugFilter(new DebugInstructionFilter());
    generator.addFliter(DebugFilter);
    GraphPtr ptr1 = generator.gen("/home/meternal/paper/test0.ll");
    GraphPtr ptr2 = generator.gen("/home/meternal/paper/test1.ll");
    assert(ptr1 && ptr2);
    cout << "gen use " << (1.0 * clock() - t) / CLOCKS_PER_SEC * 1000 << endl;
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
    sim.getSimilarity(ptr1, ptr2);
    cout << "Run time = " << (1.0 * clock() - t) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

    return 0;
}