set(SOURCE Graphgen.cpp)

add_subdirectory(GenFilter)

add_library(Graphgen ${SOURCE})
target_link_libraries(Graphgen
        GenFilter
        )