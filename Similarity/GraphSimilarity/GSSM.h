//
// Created by meternal on 4/14/20.
//

#ifndef CODESIMILARITY_GSSM_H
#define CODESIMILARITY_GSSM_H

#include "Similarity/GraphSimilarity/GraphSimilarity.hpp"
#include "Eigen/Dense"

class GSSM : public GraphSimilarity {
    using matrix = Eigen::Matrix2d;
    using matrixPtr = std::shared_ptr<matrix>;
    static constexpr double DEFAULT_CONVERGE_LIMIT = 0.03;
    static const int DEFAULT_CONVERGE_INTERVAL = 20;
    static const int DEFAULT_ITERATION_LIMIT = 1000;
    static constexpr double EPS = 0.0000001;
private:
    GraphPtr _GraphA;
    GraphPtr _GraphB;

    matrixPtr _PointMat;
    matrixPtr _EdgeMat;

    matrixPtr _SourceMatA;
    matrixPtr _TerminalMatA;
    matrixPtr _SourceMatB;
    matrixPtr _TerminalMatB;

    matrixPtr _TempPointMat;
    matrixPtr _TempEdgeMat;
private:
    void _Match(matrixPtr &_initPointMat, uint32_t _maxIteration,
                uint32_t _convergeCheckInterval = DEFAULT_CONVERGE_INTERVAL,
                double _convergeValue = DEFAULT_CONVERGE_LIMIT);

    void _InitMatStatus(matrixPtr &_initPointMat);

    void _ParseGraphAndAllocaMat();

    void _RunIterator();

    bool _IsConverge(double _convergeValue) const;

public:
    GSSM(GraphPtr &_GA, GraphPtr &_GB) {
        _GraphA = _GA;
        _GraphB = _GB;
        _ParseGraphAndAllocaMat();
    }

public:
    matrix match() override;

};


#endif //CODESIMILARITY_GSSM_H
