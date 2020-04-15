//
// Created by meternal on 4/14/20.
//

#include "GSSM.h"
#include <algorithm>

GSSM::matrix GSSM::match() {
    matrixPtr _initMat(new matrix(_GraphB->size(), _GraphA->size()));
    _initMat->fill(1);
    _Match(_initMat, DEFAULT_ITERATION_LIMIT);
    return GSSM::matrix(*_PointMat);
}

void GSSM::_Match(GSSM::matrixPtr &_initPointMat, uint32_t _maxIteration,
                  uint32_t _convergeCheckInterval,
                  double _convergeValue) {
    _ParseGraphAndAllocaMat();
    _InitMatStatus(_initPointMat);
    for (int _curr = 1; _curr <= _maxIteration; ++_curr) {
        _RunIterator();
        if (_curr % _convergeCheckInterval == 0 && _IsConverge(_convergeValue)) {
            break;
        }
    }
}

void GSSM::_InitMatStatus(GSSM::matrixPtr &_initPointMat) {
    _PointMat->swap(*_initPointMat);
    *_EdgeMat = _SourceMatB->transpose() * (*_PointMat) * (*_SourceMatA)
                + _TerminalMatB->transpose() * (*_PointMat) * (*_TerminalMatA);
}

bool GSSM::_IsConverge(double _convergeThreshold) const {
    double _maxDiff = 0;
    for (int i = 0; i < _PointMat->rows(); ++i) {
        for (int j = 0; j < _PointMat->cols(); ++j) {
            double _currVal = (*_PointMat)(i, j);
            double _oldVal = (*_TempPointMat)(i, j);
            double _currDiff = (fabs(_currDiff - _oldVal)) / (EPS + std::max(_currVal, _oldVal));
            _maxDiff = std::max(_maxDiff, _currDiff);
        }
    }
    return _maxDiff < _convergeThreshold;
}

void GSSM::_RunIterator() {
    *_TempEdgeMat = _SourceMatB->transpose() * (*_PointMat) * (*_SourceMatA)
                    + _TerminalMatB->transpose() * (*_PointMat) * (*_TerminalMatA);
    *_TempPointMat = (*_SourceMatB) * (*_EdgeMat) * _SourceMatA->transpose()
                     + (*_TerminalMatB) * (*_EdgeMat) * _TerminalMatA->transpose();
    _PointMat->swap(*_TempPointMat);
    _EdgeMat->swap(*_TempEdgeMat);
}

void GSSM::_ParseGraphAndAllocaMat() {
    assert(_GraphA && _GraphB);
    vector<std::pair<int, int>> _indexEdgeA = _GraphA->GetCurrentIndexEdge();
    vector<std::pair<int, int>> _indexEdgeB = _GraphB->GetCurrentIndexEdge();
    _PointMat.reset(new matrix(_GraphB->size(), _GraphA->size()));
    _EdgeMat.reset(new matrix(_indexEdgeB.size(), _indexEdgeA.size()));
    _SourceMatA.reset(new matrix(_GraphA->size(), _indexEdgeA.size()));
    _SourceMatB.reset(new matrix(_GraphB->size(), _indexEdgeB.size()));
    _TerminalMatA.reset(new matrix(_GraphA->size(), _indexEdgeA.size()));
    _TerminalMatB.reset(new matrix(_GraphB->size(), _indexEdgeB.size()));
    _TempPointMat.reset(new matrix(_GraphB->size(), _GraphA->size()));
    _TempEdgeMat.reset(new matrix(_indexEdgeB.size(), _indexEdgeA.size()));
    _PointMat->fill(0);
    _EdgeMat->fill(0);
    _SourceMatA->fill(0);
    _SourceMatB->fill(0);
    _TerminalMatA->fill(0);
    _TerminalMatB->fill(0);
    _TempPointMat->fill(0);
    _TempEdgeMat->fill(0);
    for (int i = 0; i < _indexEdgeA.size(); ++i) {
        (*_SourceMatA)(_indexEdgeA[i].first, i) = 1;
        (*_TerminalMatA)(_indexEdgeA[i].second, i) = 1;
    }
    for (int i = 0; i < _indexEdgeB.size(); ++i) {
        (*_SourceMatB)(_indexEdgeB[i].first, i) = 1;
        (*_TerminalMatB)(_indexEdgeB[i].second, i) = 1;
    }
}