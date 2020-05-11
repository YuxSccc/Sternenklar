//
// Created by Meternal on 2020/5/10.
//

#include "OutputAnalyzer.h"

#include "graphviz/gvc.h"

#include <sstream>
#include <iomanip>
#include <filesystem>

namespace ster {
    void OutputAnalyzer::OutputAnalyzeImage(const GraphPtr &_lhs, const GraphPtr &_rhs,
                                            const vector<std::pair<int, double>> &_left_match, double _similarity_value,
                                            const string &_output_dir) {
        int _argc = 4;
        char *_argv[] = {
                const_cast<char *>("dot"),
                const_cast<char *>("-Tpng"),
                const_cast<char *>("-o"),
                const_cast<char *>(_output_dir.c_str())
        };
        Agraph_t *graph, *sub_lhs, *sub_rhs;
        vector<Agedge_t *> _match_edge_vector;
        vector<Agnode_t *> _left_agnodes;
        vector<Agnode_t *> _right_agnodes;

        GVC_t *gvc;
        gvc = gvContext();
        gvParseArgs(gvc, _argc, _argv);

        graph = agopen(const_cast<char *>("graph"), Agdirected, 0);
        sub_lhs = agsubg(graph, const_cast<char *>("cluster_sub_left"), 1);
        sub_rhs = agsubg(graph, const_cast<char *>("cluster_sub_right"), 1);

        agattr(graph, AGRAPH, const_cast<char *>("dpi"), const_cast<char *>("200"));

        agattr(graph, AGRAPH, const_cast<char *>("label"), const_cast<char *>("Analyze"));
        agattr(graph, AGRAPH, const_cast<char *>("rankdir"), const_cast<char *>("LR"));
        agattr(graph, AGRAPH, const_cast<char *>("bgcolor"), const_cast<char *>("white"));
        // agattr(graph, AGEDGE, "constraint", "false");

        agattr(sub_lhs, AGRAPH, const_cast<char *>("label"), get_filename_from_dir(_lhs->get_source_filename()).data());
        agattr(sub_lhs, AGRAPH, const_cast<char *>("labelloc"), const_cast<char *>("t"));
        agattr(sub_lhs, AGRAPH, const_cast<char *>("bgcolor"), const_cast<char *>("white"));
        agattr(sub_lhs, AGNODE, const_cast<char *>("shape"), const_cast<char *>("polygon"));
        agattr(sub_lhs, AGRAPH, const_cast<char *>("rankdir"), const_cast<char *>("BT"));

        agattr(sub_rhs, AGRAPH, const_cast<char *>("label"), get_filename_from_dir(_rhs->get_source_filename()).data());
        agattr(sub_rhs, AGRAPH, const_cast<char *>("labelloc"), const_cast<char *>("t"));
        agattr(sub_rhs, AGRAPH, const_cast<char *>("bgcolor"), const_cast<char *>("white"));
        agattr(sub_rhs, AGNODE, const_cast<char *>("shape"), const_cast<char *>("polygon"));
        agattr(sub_rhs, AGRAPH, const_cast<char *>("rankdir"), const_cast<char *>("BT"));

        std::stringstream ss;
        auto _get_node_name = [&ss](const string &_prefix, int index) {
            ss.str("");
            ss << _prefix << index;
            return ss.str();
        };
        for (int i = 0; i < (int) _lhs->size(); ++i)
            _left_agnodes.push_back(agnode(sub_lhs, _get_node_name("L", i).data(), 1));
        for (int i = 0; i < (int) _rhs->size(); ++i)
            _right_agnodes.push_back(agnode(sub_rhs, _get_node_name("R", i).data(), 1));

        auto _get_source_code = [&ss](const vector<std::pair<int, string>> &_temp_vector) {
            ss.str("");
            for (const auto &_iter : _temp_vector) {
                ss << _iter.first << ":" << _iter.second << "\\l";
            }
            return ss.str();
        };
        for (int i = 0; i < (int) _lhs->size(); ++i) {
            agset(_left_agnodes[i], const_cast<char *>("label"),
                  _get_source_code(_lhs->getNodePtrAt(i)->get_source_code()).data());
        }
        for (int i = 0; i < (int) _rhs->size(); ++i) {
            agset(_right_agnodes[i], const_cast<char *>("label"),
                  _get_source_code(_rhs->getNodePtrAt(i)->get_source_code()).data());
        }

        auto _get_weight_percent = [&ss](double _temp_val) {
            ss.str("");
            ss << std::setprecision(2) << std::setiosflags(std::ios::fixed) << _temp_val * 100 << "%\n";
            return ss.str();
        };

        for (int i = 0; i < (int) _left_match.size(); ++i) {
            if (_left_match[i].first != -1) {
                int _left_index = i;
                int _right_index = _left_match[i].first;
                _match_edge_vector.push_back(agedge(graph, _left_agnodes[_left_index], _right_agnodes[_right_index],
                                                    _get_node_name("ME", i).data(), 1));
                agsafeset(*_match_edge_vector.rbegin(), const_cast<char *>("label"),
                          _get_weight_percent(_left_match[i].second).data(),
                          const_cast<char *>(""));
                agsafeset(*_match_edge_vector.rbegin(), const_cast<char *>("color"), const_cast<char *>("red"),
                          const_cast<char *>("red"));
                agsafeset(*_match_edge_vector.rbegin(), const_cast<char *>("penwidth"), const_cast<char *>("1.5"),
                          const_cast<char *>("1.0"));
            }
        }

        Agnode_t *_summary = agnode(graph, const_cast<char *>("Summary"), 1);
        agset(_summary, const_cast<char *>("shape"), const_cast<char *>("Msquare"));
        agset(_summary, const_cast<char *>("label"), ("Similarity = " + _get_weight_percent(_similarity_value)).data());

        // if (!_left_agnodes.empty()) agedge(graph, _summary, *_left_agnodes.rbegin(), "STOL", 1);
        // if (!_right_agnodes.empty()) agedge(graph, _summary, *_right_agnodes.rbegin(), "STOR", 1);

        gvLayoutJobs(gvc, graph);
        gvRenderJobs(gvc, graph);
        gvFreeLayout(gvc, graph);
        agclose(graph);
        LOG_IF(FATAL, gvFreeContext(gvc)) << "Free gvc context ERROR!\n";
        LOG(INFO) << "Generate PNG analyze file success in " << _output_dir.data() << "\n";
    }

    void OutputAnalyzer::OutputAnalyzeImage(const GraphPtr &_lhs, const GraphPtr &_rhs,
                                            const vector<std::pair<int, double>> &_left_match,
                                            double _similarity_value) {
        if (!std::filesystem::exists("analysis_folder")) {
            std::filesystem::create_directory("analysis_folder");
        }
        std::stringstream ssm;
        string _lhs_filename = get_filename_from_dir(_lhs->get_source_filename());
        string _rhs_filename = get_filename_from_dir(_rhs->get_source_filename());
        ssm << "analysis_folder/";
        ssm << std::setprecision(4) << std::setiosflags(std::ios::fixed) << _similarity_value * 100;
        ssm << "_" << _lhs_filename << "_" << _rhs_filename << ".png";
        OutputAnalyzeImage(_lhs, _rhs, _left_match, _similarity_value, ssm.str());
    }
}