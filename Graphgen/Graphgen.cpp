//
// Created by meternal on 4/3/20.
//

#include "Eigen/Dense"

#include "Graphgen.h"

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/DebugInfo.h"

#include <unordered_map>

int Graphgen::addFliter(GenFilterPtr ptr) {
    _Filters.push_back(ptr);
}

GraphPtr Graphgen::gen(string filename) const {
    GraphPtr graph;
    // TODO: file class?

    llvm::SMDiagnostic Err;
    llvm::LLVMContext Context;
    // Parse the input LLVM IR file into a module.
    std::unique_ptr<llvm::Module> Mod(parseIRFile(filename.c_str(), Err, Context));
    llvm::Module *M = Mod.get();

    Node *Graph_entry = nullptr;
    vector<Node *> Graph_nodes;
    std::unordered_map<Node *, vector<Node *>> Graph_edges;

    // main process
    // TODO: error handle
    for (llvm::Function &F : *M) {
        for (llvm::BasicBlock &BB: F) {
            vector<Instruction *> insList;
            for (llvm::Instruction &I : BB) {
                insList.push_back(new Instruction(I));
            }
            for (const GenFilterPtr &Filter : _Filters) {
                Filter->Filter(insList);
            }
            Graph_nodes.push_back(new Node(insList));
        }
    }
    graph->initGraph(Graph_entry, Graph_nodes, Graph_edges);
    return graph;
}

int Graphgen::removeFliter(int index) {
    assert(index < _Filters.size());
    _Filters.erase(_Filters.begin() + index);
}