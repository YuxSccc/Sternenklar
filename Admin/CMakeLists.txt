set(SOURCE AdminConfig.cpp CSAdmin.cpp)

add_library(Admin ${SOURCE})

target_link_libraries(Admin
        Graphgen
        Similarity
        LLVM-9
        glog
        )