//
// Created by meternal on 4/22/20.
//

#include "Runner.h"

#include "Runner/RunnerConfig.h"

#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

namespace ster {
    void Runner::run(const string &_run_path,
                     const string &_input_file,
                     const string &_output_file,
                     Runner::RUN_MODE _mode) {
        RunnerConfig _config;
        switch (_mode) {
            case Runner::RUN_MODE::CLANG_COMPILE:
                _config.load_cpp_compile_information(_run_path, _input_file, _output_file);
                break;
            case Runner::RUN_MODE::LLVM_OPT:
                _config.load_llvm_opt_information(_run_path, _input_file, _output_file);
                break;
            default:
                LOG(FATAL) << "Can not handle RUN_MODE\n";
        }
        pid_t _pid = fork();
        LOG_IF(FATAL, _pid < 0);
        if (0 == _pid) {
            // child process
            int _exit_code = execvp(_run_path.data(), _config.get_param());
            LOG(FATAL) << "Child process error: EXIT CODE " << _exit_code << "\n";
        } else {
            // main process
            int _child_status;
            waitpid(_pid, &_child_status, 0);
            if (WIFEXITED(_child_status)) {
                int _exit_status = WEXITSTATUS(_child_status);
                LOG_IF(FATAL, _exit_status != 0) << "Exit status of the child was" << _exit_status << "\n"
                                                 << _config << "\n";
            }
        }
    }
}