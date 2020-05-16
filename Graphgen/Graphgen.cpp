//
// Created by meternal on 4/3/20.
//

#include "Graphgen.h"

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/DebugInfo.h"

#include <unordered_map>

#include "Eigen/Dense"
#include <fstream>

namespace ster {
    int Graphgen::addFliter(GenFilterPtr ptr) {
        _Filters.push_back(ptr);
        return 0;
    }

    GraphPtr Graphgen::gen(string filename, string _source_code_filename) const {
        GraphPtr graph(new Graph());
        // TODO: file class?

        vector<string> _graph_source_code;
        std::ifstream ifs;
        ifs.open(_source_code_filename);
        LOG_IF(FATAL, !ifs.is_open()) << "Can't get source code from " << _source_code_filename << "\n";
        {
            string _temp;
            while (getline(ifs, _temp)) {
                _graph_source_code.emplace_back(std::move(_temp));
            }
        }
        ifs.close();

        llvm::SMDiagnostic Err;
        llvm::LLVMContext Context;
        // Parse the input LLVM IR file into a module.
        std::unique_ptr<llvm::Module> Mod(parseIRFile(filename.c_str(), Err, Context));
        if (!Mod) return nullptr;
        llvm::Module *M = Mod.get();

        Node *Graph_entry = nullptr;
        vector<Node *> Graph_nodes;
        std::unordered_map<Node *, vector<Node *>> Graph_edges;
        std::unordered_map<llvm::BasicBlock *, Node *> _tempBBMap;

        // main process
        // TODO: error handle
        for (llvm::Function &F : *M) {
            if (F.getName() != "main" && F.use_empty()) continue;
            for (llvm::BasicBlock &BB: F) {
                vector<Instruction *> insList;
                std::map<Instruction *, int> _Ins_to_Line;
                for (llvm::Instruction &I : BB) {
                    if (llvm::DILocation *Loc = I.getDebugLoc()) {
                        string _loc_filename = get_filename_from_dir(Loc->getFilename());
                        string _module_filename = get_filename_from_dir(M->getSourceFileName());
                        if (_module_filename == _loc_filename) {
                            insList.push_back(new Instruction(I));
                            _Ins_to_Line[*insList.rbegin()] = Loc->getLine() - 1;
                        }
                    }
                }
                for (const GenFilterPtr &Filter : _Filters) {
                    Filter->Filter(insList);
                }
                if (insList.empty())continue;

                // debug code
                string _temp;
                llvm::raw_string_ostream rso(_temp);
                BB.print(rso);

                std::set<int> _LineSet;
                for (auto _ptr : insList) {
                    auto _iter = _Ins_to_Line.find(_ptr);
                    if (_iter != _Ins_to_Line.end()) {
                        _LineSet.insert(_iter->second);
                    }
                }

                vector<std::pair<int, string>> _source_vector;
                for (auto _Line : _LineSet) {
                    if (_Line >= 0 && _Line < _graph_source_code.size())
                        _source_vector.push_back(make_pair(_Line, _graph_source_code[_Line]));
                }

                Graph_nodes.push_back(new Node(insList, _temp, std::move(_source_vector)));
                _tempBBMap[&BB] = *Graph_nodes.rbegin();
            }
        }

        for (llvm::Function &F : *M) {
            for (llvm::BasicBlock &BB: F) {
                auto _currBBNodeIter = _tempBBMap.find(&BB);
                if (_currBBNodeIter == _tempBBMap.end()) continue;
                auto _currBBNode = _currBBNodeIter->second;
                for (llvm::BasicBlock *succPtr : llvm::successors(&BB)) {
                    auto _currSuccBBNodeIter = _tempBBMap.find(succPtr);
                    if (_currSuccBBNodeIter == _tempBBMap.end()) continue;
                    auto _currSuccBBNode = _currSuccBBNodeIter->second;
                    Graph_edges[_currBBNode].push_back(_currSuccBBNode);
                }
            }
        }

        if (Graph_nodes.empty()) {
            LOG(WARNING) << "Can not parse any node in " << filename << "\n";
            return nullptr;
        }

        Graph_entry = Graph_nodes[0];
        graph->initGraph(Graph_entry, Graph_nodes, Graph_edges, filename);
        return graph;
    }

    int Graphgen::removeFliter(size_t index) {
        assert(index < _Filters.size());
        _Filters.erase(_Filters.begin() + index);
        return 0;
    }
}