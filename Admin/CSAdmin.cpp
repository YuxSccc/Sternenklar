//
// Created by meternal on 4/14/20.
//

#include "CSAdmin.h"

#include <memory>

#include "Runner/Runner.h"
#include "Graphgen/Graphgen.h"
#include "Similarity/Similarity.h"
#include "Graphgen/GenFilter/DebugInstructionFilter.h"
#include "Similarity/OutputAnalyzer/OutputAnalyzer.h"
#include "utils/ThreadPool.h"

#include "sys/time.h"

namespace ster {
    void CSAdmin::load_config(AdminConfig _load_config) {
        _config = std::move(_load_config);
    }

    void CSAdmin::run() {
        // TODO: add opt-9
        // TODO: divide function
        struct timeval start, end, compilefinish, graphgenfinish, simfinish;
        gettimeofday(&start, NULL);

        const vector<std::filesystem::path> &_source_list = _config.get_source_code_list_refrence();

        Graphgen _gen;
        GenFilterPtr DebugFilter(new DebugInstructionFilter());
        _gen.addFliter(DebugFilter);

        Similarity _sim;
        vector<SimilarityResult> _results;
        ThreadPool pool(_config.get_thread_number());

        // compile
        vector<std::future<int>> _compile_future(_source_list.size());
        vector<string> _target_compile_path;
        for (auto &_item:_source_list) {
            std::filesystem::path _out_path = _config.get_IR_folder_path().string() + "/" +
                                              _item.filename().replace_extension("CSLL").string();
            _target_compile_path.push_back(std::move(_out_path));
        }

        for (int i = 0; i < _source_list.size(); ++i) {
            _compile_future[i] = pool.enqueue(Runner::run, _config.get_compiler_path(), _source_list[i],
                                              _target_compile_path[i],
                                              Runner::RUN_MODE::CLANG_COMPILE);
        }
        for (int i = 0; i < _compile_future.size(); ++i) {
            _compile_future[i].wait();
            if (_compile_future[i].get() == 0) {
                _config.add_IR_list_element(_target_compile_path[i], _source_list[i]);
                LOG(INFO) << "Compile " << _source_list[i].filename() << " Success.\n";
            } else {
                _compile_fail_filename.push_back(_source_list[i].filename().string());
                LOG(INFO) << "Compile " << _source_list[i].filename() << " Failed.\n";
            }
        }
        gettimeofday(&compilefinish, NULL);

        // graphgen
        auto _ir_path_list = _config.get_IR_list_refrence();
        vector<std::future<int>> _graph_future(_ir_path_list.size());
        vector<GraphPtr> _graph_list(_ir_path_list.size());
        for (auto &it : _graph_list) it = std::make_shared<Graph>();
        for (int i = 0; i < _ir_path_list.size(); ++i) {
            _graph_future[i] = pool.enqueue(&Graphgen::gen, std::ref(_gen), _ir_path_list[i].first,
                                            _ir_path_list[i].second, std::ref(_graph_list[i]));
        }
        for (int i = 0; i < _graph_future.size(); ++i) {
            _graph_future[i].wait();
            if (_graph_future[i].get()) {
                LOG(WARNING) << "Gen graph for code " << _ir_path_list[i].second << " failed, skip.\n";
                continue;
            }
            LOG_EVERY_N(INFO, 50) << "Gen graph for code " << _ir_path_list[i].second << " successful.\n";
        }
        {
            vector<GraphPtr> _temp_swap_graph_list;
            for (auto &it : _graph_list) {
                if (it) _temp_swap_graph_list.push_back(it);
            }
            _graph_list.swap(_temp_swap_graph_list);
        }
        gettimeofday(&graphgenfinish, NULL);

        // sim
        size_t _sim_size = _graph_list.size() * (_graph_list.size() - 1) / 2;
        vector<vector<std::pair<int, double>>> _left_match_pack(_sim_size);
        vector<std::future<double>> _similarity_future(_sim_size);

        for (int i = 0, _future_index = 0; i < (int) _graph_list.size(); ++i) {
            for (int j = i + 1; j < (int) _graph_list.size(); ++j, ++_future_index) {
                _similarity_future[_future_index] = pool.enqueue(&Similarity::getSimilarity, _sim,
                                                                 std::ref(_graph_list[i]), std::ref(_graph_list[j]),
                                                                 std::ref(_left_match_pack[_future_index]));
            }
        }
        for (int i = 0, _future_index = 0; i < (int) _graph_list.size(); ++i) {
            for (int j = i + 1; j < (int) _graph_list.size(); ++j, ++_future_index) {
                _similarity_future[_future_index].wait();
                _results.emplace_back(SimilarityResult(
                        _graph_list[i]->get_source_filename(), _graph_list[j]->get_source_filename(),
                        _similarity_future[_future_index].get()));
                LOG_EVERY_N(INFO, 50) << "Calculate similarity between "
                                      << get_filename_from_dir(_graph_list[i]->get_source_filename()) << " and "
                                      << get_filename_from_dir(_graph_list[j]->get_source_filename())
                                      << " successful.\n";
            }
        }
        gettimeofday(&simfinish, NULL);

        std::sort(_results.begin(), _results.end());
        std::reverse(_results.begin(), _results.end());
        LOG(INFO) << "Output result" << "\n";
        for (const auto &it : _results) {
            LOG(INFO) << it << "\n";
        }

        LOG(INFO) << "Compile Failed " << _compile_fail_filename.size() << " files\n";
        for (const auto &it : _compile_fail_filename) {
            LOG(INFO) << it << "\t";
        }
        LOG(INFO) << "\n";

        gettimeofday(&end, NULL);
        int bbsize = 0;
        int inssize = 0;
        for (auto &it : _graph_list) {
            bbsize += it->size();
            for (int i = 0; i < it->size(); ++i) {
                inssize += it->getNodePtrAt(i)->size();
            }
        }

        LOG(INFO) << "Runtime = "
                  << end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_usec / 1000 - start.tv_sec * 1000 << "ms\n";
        LOG(INFO) << "Compiletime = "
                  << compilefinish.tv_sec * 1000 + compilefinish.tv_usec / 1000 - start.tv_usec / 1000 -
                     start.tv_sec * 1000
                  << "ms\n";

        LOG(INFO) << "Graphgentime = "
                  << graphgenfinish.tv_sec * 1000 + graphgenfinish.tv_usec / 1000 - compilefinish.tv_usec / 1000 -
                     compilefinish.tv_sec * 1000
                  << "ms\n";

        LOG(INFO) << "Simtime = "
                  << simfinish.tv_sec * 1000 + simfinish.tv_usec / 1000 - graphgenfinish.tv_usec / 1000 -
                     graphgenfinish.tv_sec * 1000
                  << "ms\n";

        LOG(INFO) << "code count" << _graph_list.size() << "\n";
        LOG(INFO) << "bbsize = " << bbsize << " avg bbsize = " << 1.0 * bbsize / _graph_list.size() << "\n";
        LOG(INFO) << "inssize = " << inssize << " avg inssize = " << 1.0 * inssize / bbsize << "\n";
    }
}