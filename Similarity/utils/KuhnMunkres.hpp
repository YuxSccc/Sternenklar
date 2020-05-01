//
// Created by Meternal on 2020/4/11.
//

#ifndef CODESIMILARITY_KUHNMUNKRES_HPP
#define CODESIMILARITY_KUHNMUNKRES_HPP

#include <vector>
#include <queue>
#include <algorithm>

using std::vector;

class KuhnMunkres {
private:
    enum class Type {
        UNMATCH = -1
    };

    class KM_Node {
    public:
        double _nodeWeight;
        bool _Visited;
        int _matchIndex;
        vector<double> _edges;

        explicit KM_Node(size_t _edgeCount) : _nodeWeight(0), _Visited(false),
                                              _matchIndex((int) Type::UNMATCH) {
            _edges.resize(_edgeCount, 0);
        }

        inline bool isMatched() {
            return _matchIndex != (int) Type::UNMATCH;
        }
    };

    /*
    class KM_RightNode : public KM_Node {
        double _slack;
        int _prevIndex;
    };
    */

private:
    constexpr static double SLACK_INF = 1e20;

    vector<KM_Node> _leftNodes;
    vector<KM_Node> _rightNodes;

    vector<double> _slack;
    vector<int> _prevIndex;
    std::queue<int> _bfsQueue;

private:
    void augment(int rightIndex) {
        _rightNodes[rightIndex]._matchIndex = _prevIndex[rightIndex];
        if (_leftNodes[_prevIndex[rightIndex]].isMatched()) augment(_leftNodes[_prevIndex[rightIndex]]._matchIndex);
        _leftNodes[_rightNodes[rightIndex]._matchIndex]._matchIndex = rightIndex;
    }

    void KM_bfs(int s) {
        std::fill(_slack.begin(), _slack.end(), SLACK_INF);
        for (auto &it : _leftNodes) it._Visited = false;
        for (auto &it : _rightNodes) it._Visited = false;
        while (!_bfsQueue.empty()) {
            _bfsQueue.pop();
        }
        _bfsQueue.push(s);
        while (true) {
            while (!_bfsQueue.empty()) {
                int headIndex = _bfsQueue.front();
                _bfsQueue.pop();
                auto &head = _leftNodes[headIndex];
                head._Visited = true;
                for (uint32_t i = 0; i < _rightNodes.size(); ++i) {
                    auto &currRight = _rightNodes[i];
                    if (!currRight._Visited) {
                        double diff = head._nodeWeight + currRight._nodeWeight - head._edges[i];
                        if (0 == diff) {
                            currRight._Visited = true;
                            _prevIndex[i] = headIndex;
                            if (!currRight.isMatched()) {
                                augment(i);
                                return;
                            }
                            _bfsQueue.push(currRight._matchIndex);
                            continue;
                        }
                        if (_slack[i] > diff) {
                            _slack[i] = diff;
                            _prevIndex[i] = headIndex;
                        }
                    }
                }
            }
            double delta = SLACK_INF;
            int tempIndex = 0;
            for (auto &item: _rightNodes) {
                if (!item._Visited) {
                    delta = std::min(delta, _slack[tempIndex]);
                }
                tempIndex++;
            }
            for (auto &item : _leftNodes) {
                if (item._Visited) {
                    item._nodeWeight -= delta;
                }
            }
            tempIndex = 0;
            for (auto &item : _rightNodes) {
                if (item._Visited) {
                    item._nodeWeight += delta;
                } else {
                    _slack[tempIndex] -= delta;
                }
                tempIndex++;
            }
            for (size_t i = 0; i < _rightNodes.size(); ++i) {
                auto &item = _rightNodes[i];
                if (!item._Visited && _slack[i] == 0) {
                    item._Visited = true;
                    if (!item.isMatched()) {
                        augment(i);
                        return;
                    }
                    _bfsQueue.push(item._matchIndex);
                }
            }
        }
    }

    void _init_KM() {
        while (!_bfsQueue.empty()) _bfsQueue.pop();
        _prevIndex.resize(_rightNodes.size());
        _slack.resize(_rightNodes.size());
        std::fill(_prevIndex.begin(), _prevIndex.end(), 0);
        for (auto &it : _leftNodes) {
            it._matchIndex = (int) Type::UNMATCH;
            it._nodeWeight = *std::max_element(it._edges.begin(), it._edges.end());
        }

        for (auto &it : _rightNodes) {
            it._matchIndex = (int) Type::UNMATCH;
            it._nodeWeight = 0;
        }
    }

    double _Match(vector<std::pair<int, double>> &_leftMatch) {
        _init_KM();
        _leftMatch.resize(_leftNodes.size());
        for (size_t matchCount = 0; matchCount < _leftNodes.size(); ++matchCount) {
            KM_bfs(matchCount);
        }
        double sumMatchWeight = 0;
        for (size_t i = 0; i < _leftNodes.size(); ++i) {
            double _match_weight = _leftNodes[i]._nodeWeight + _rightNodes[_leftNodes[i]._matchIndex]._nodeWeight;
            _leftMatch[i] = std::make_pair(_leftNodes[i]._matchIndex, _match_weight);
            sumMatchWeight += _match_weight;
        }
        return sumMatchWeight;
    }

    double _PreMatch(size_t _leftCount, size_t _rightCount, const vector<std::pair<int, int>> &_edgeIndex,
                     const vector<double> &_edgeWeight, vector<std::pair<int, double>> &_leftMatch) {
        {
            vector<KM_Node> tempa, tempb;
            std::swap(tempa, _leftNodes);
            std::swap(tempb, _rightNodes);
        }
        _leftNodes.resize(_leftCount, KM_Node(_rightCount));
        _rightNodes.resize(_rightCount, KM_Node(_leftCount));

        assert(_edgeIndex.size() == _edgeWeight.size());
        for (uint32_t i = 0; i < _edgeIndex.size(); ++i) {
            auto &tempEdge = _edgeIndex[i];
            auto weight = _edgeWeight[i];
            int leftIndex = tempEdge.first;
            int rightIndex = tempEdge.second;
            _leftNodes[leftIndex]._edges[rightIndex] = weight;
            _rightNodes[rightIndex]._edges[leftIndex] = weight;
        }
        bool swapFlag = false;
        if (_leftCount > _rightCount) {
            swapFlag = true;
            std::swap(_leftCount, _rightCount);
            std::swap(_leftNodes, _rightNodes);
        }
        double sumWeight = _Match(_leftMatch);
        if (swapFlag) {
            vector<std::pair<int, double>> newMatch(_rightCount, std::make_pair(-1, -1));
            for (size_t i = 0; i < _leftMatch.size(); ++i) {
                newMatch[_leftMatch[i].first] = std::make_pair(i, _leftMatch[i].second);
            }
            std::swap(_leftMatch, newMatch);
        }
        return sumWeight;
    }

public:
    double Match(size_t _leftCount, size_t _rightCount, const vector<std::pair<int, int>> &_edgeIndex,
                 const vector<double> &_edgeWeight, vector<std::pair<int, double>> &_leftMatch) {
        return _PreMatch(_leftCount, _rightCount, _edgeIndex, _edgeWeight, _leftMatch);
    }

    double Match(size_t _leftCount, size_t _rightCount, const vector<std::pair<int, int>> &_edgeIndex,
                 const vector<double> &_edgeWeight) {
        vector<std::pair<int, double>> temp;
        return _PreMatch(_leftCount, _rightCount, _edgeIndex, _edgeWeight, temp);
    }
};


#endif //CODESIMILARITY_KUHNMUNKRES_HPP
