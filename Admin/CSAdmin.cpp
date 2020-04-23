//
// Created by meternal on 4/14/20.
//

#include "CSAdmin.h"

#include "Runner/Runner.h"
#include "Graphgen/Graphgen.h"
#include "Similarity/Similarity.h"
#include "Graphgen/GenFilter/DebugInstructionFilter.h"

namespace ster {
    void CSAdmin::load_config(AdminConfig _load_config) {
        _config = std::move(_load_config);
    }

    void CSAdmin::run() {
        // TODO: add opt-9
        // TODO: divide function
        clock_t enter_run_time = clock();

        const vector<std::filesystem::path> &_source_list = _config.get_source_code_list_refrence();
        vector<GraphPtr> _graph_list;

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
                _runner.run(_config.get_compiler_path(),
                            _item.string(),
                            _out_path.string(),
                            Runner::RUN_MODE::CLANG_COMPILE);

                _config.add_IR_list_element(_out_path);
            }

            auto _IR_list = _config.get_IR_list_refrence();

            for (auto &_item : _IR_list) {
                auto _gPtr = _gen.gen(_item);
                if (!_gPtr) {
                    LOG(WARNING) << "Gen graph for code " << _item << " failed, skip.\n";
                    continue;
                }
                LOG(INFO) << "Gen graph for code " << _item << " successful.\n";
                _graph_list.push_back(std::move(_gPtr));
            }
            for (int i = 0; i < (int) _graph_list.size(); ++i) {
                for (int j = i + 1; j < (int) _graph_list.size(); ++j) {
                    LOG(INFO) << "Calculate similarity between " << _graph_list[i]->get_source_filename() << " and "
                              << _graph_list[j]->get_source_filename()
                              << " Start.\n";
                    _results.emplace_back(SimilarityResult(
                            _graph_list[i]->get_source_filename(), _graph_list[j]->get_source_filename(),
                            _sim.getSimilarity(_graph_list[i], _graph_list[j])));
                    LOG(INFO) << "Calculate similarity between " << _graph_list[i]->get_source_filename() << " and "
                              << _graph_list[j]->get_source_filename()
                              << " successful.\n\n";
                }
            }
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

        clock_t exit_run_time = clock();
        LOG(INFO) << "Runtime = " << 1.0 * (exit_run_time - enter_run_time) / CLOCKS_PER_SEC * 1000 << "ms\n";
    }
}