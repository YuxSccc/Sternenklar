set(SOURCE AdminConfig.cpp CSAdmin.cpp)

add_library(Admin STATIC ${SOURCE})

#add_definitions(-DGFLAGS_DLL_DECLARE_FLAG
#        -DGFLAGS_DLL_DEFINE_FLAG
#        -DGLOG_NO_ABBREVIATED_SEVERITIES
#        -DGOOGLE_GLOG_DLL_DECL)

target_link_libraries(Admin
        Graphgen
        Similarity
        Runner
        ${llvm_libs}
        glog
        )