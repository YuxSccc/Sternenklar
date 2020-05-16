//
// Created by meternal on 4/14/20.
//

#include "CSAdmin.h"

#include "Runner/Runner.h"
#include "Graphgen/Graphgen.h"
#include "Similarity/Similarity.h"
#include "Graphgen/GenFilter/DebugInstructionFilter.h"
#include "Similarity/OutputAnalyzer/OutputAnalyzer.h"
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
        vector<GraphPtr> _graph_list;

        int sourcesum = 0;

        Graphgen _gen;
        GenFilterPtr DebugFilter(new DebugInstructionFilter());
        _gen.addFliter(DebugFilter);

        Similarity _sim;

        vector<SimilarityResult> _results;

        if (!_config.is_multithreading()) {
            for (auto &_item:_source_list) {
                Runner _runner;
                std::filesystem::path _out_path = _config.get_IR_folder_path().string() + "/" +
                                                  _item.filename().replace_extension("CSLL").string();
                int _run_status = 0;
                if (!std::filesystem::exists(_out_path)) {
                    _run_status = _runner.run(_config.get_compiler_path(),
                                              _item.string(),
                                              _out_path.string(),
                                              Runner::RUN_MODE::CLANG_COMPILE);
                }
                if (_run_status == 0) {
                    _config.add_IR_list_element(_out_path, _item);
                    LOG(INFO) << "Compile " << _item.filename() << " Success.\n";
                } else {
                    _compile_fail_filename.push_back(_item.filename().string());
                    LOG(INFO) << "Compile " << _item.filename() << " Failed.\n";
                }
            }
            gettimeofday(&compilefinish, NULL);

            auto _IR_list = _config.get_IR_list_refrence();

            for (auto &_item : _IR_list) {
                auto _gPtr = _gen.gen(_item.first, _item.second, sourcesum);
                if (!_gPtr) {
                    LOG(WARNING) << "Gen graph for code " << _item.second << " failed, skip.\n";
                    continue;
                }
                LOG_EVERY_N(INFO, 50) << "Gen graph for code " << _item.second
                                      << " and other 49 instances successful.\n";
                _graph_list.push_back(std::move(_gPtr));
            }
            gettimeofday(&graphgenfinish, NULL);

            for (int i = 0; i < (int) _graph_list.size(); ++i) {
                for (int j = i + 1; j < (int) _graph_list.size(); ++j) {
                    vector<std::pair<int, double>> _left_match;

                    _results.emplace_back(SimilarityResult(
                            _graph_list[i]->get_source_filename(), _graph_list[j]->get_source_filename(),
                            _sim.getSimilarity(_graph_list[i], _graph_list[j], _left_match)));

                    if (_results.rbegin()->get_similarity() > 0.0) {
                        OutputAnalyzer::OutputAnalyzeImage(_graph_list[i], _graph_list[j], _left_match,
                                                           _results.rbegin()->get_similarity());
                    }

                    LOG_EVERY_N(INFO, 50) << "Calculate similarity between "
                                          << get_filename_from_dir(_graph_list[i]->get_source_filename()) << " and "
                                          << get_filename_from_dir(_graph_list[j]->get_source_filename())
                                          << " and other 49 instances successful.\n";
                }
            }
            gettimeofday(&simfinish, NULL);

        } else {
            // TODO: dev multiprocessing
            assert(false);
        }

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
        LOG(INFO) << "Runtime = "
                  << end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_usec / 1000 - start.tv_sec * 1000 << "ms\n";

        int bbsize = 0;
        int inssize = 0;
        for (auto &it : _graph_list) {
            bbsize += it->size();
            for (int i = 0; i < it->size(); ++i) {
                inssize += it->getNodePtrAt(i)->size();
            }
        }
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
        LOG(INFO) << "source line " << sourcesum << "\n";
        LOG(INFO) << "bbsize = " << bbsize << " avg bbsize = " << 1.0 * bbsize / _graph_list.size() << "\n";
        LOG(INFO) << "inssize = " << inssize << " avg inssize = " << 1.0 * inssize / bbsize << "\n";
    }
}