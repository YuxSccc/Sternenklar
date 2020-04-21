cmake_minimum_required(VERSION 3.12)
project(CodeSimilarity)

set(CMAKE_CXX_STANDARD 17)


add_compile_options(-Wall)
message(STATUS "${DCMAKE_CXX_FLAGS}")

set(CMAKE_BUILD_TYPE RELEASE)

if (DEBUG)
    set(DCMAKE_CXX_FLAGS ${DCMAKE_CXX_FLAGS} -pg)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -pg)
endif ()

include_directories(
        .
        ./include
        /usr/local/include/eigen3
        /usr/include/llvm-9
        /usr/include/llvm-c-9
)

add_subdirectory(Graphgen)
add_subdirectory(Similarity)
add_subdirectory(Admin)

link_directories(/usr/lib/llvm-9/lib /usr/local/lib)

add_executable(CodeSimilarity main.cpp)

target_link_libraries(CodeSimilarity
        Admin
        )
