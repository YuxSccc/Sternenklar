#include "Admin/CSAdmin.h"

#include "Admin/AdminConfig.h"

#include <filesystem>
#include "utils/cxxopts.hpp"

ster::AdminConfig parseParam(int _argc, char *_argv[]) {
    ster::AdminConfig _config;
    cxxopts::Options options("Sternenklar", "Code Similarity tool");
    options.add_options()
            ("j,jobs", "Multithreading, Number of threads", cxxopts::value<uint32_t>())
            ("f,folder", "Source code folder name", cxxopts::value<std::vector<std::string>>())
            ("h,help", "Print usage");

    auto result = options.parse(_argc, _argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (result.count("jobs")) {
        _config.set_thread_number(result["jobs"].as<uint32_t>());
    }
    if (result.count("folder")) {
        std::vector<std::string> _folders = result["folder"].as<std::vector<std::string>>();
        for (const auto &_item : _folders) {
            _config.add_filename_from_folder(_item);
        }
    }
    return _config;
}


int main(int argc, char *argv[]) {
    ster::CSAdminPtr admin(new ster::CSAdmin());
    admin->load_config(parseParam(argc, argv));
    admin->run();
    return 0;
}