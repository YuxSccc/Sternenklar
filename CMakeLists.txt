cmake_minimum_required(VERSION 3.12)
project(CodeSimilarity)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall)
set(CMAKE_BUILD_TYPE RELEASE)

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

link_directories(/usr/lib/llvm-9/)

add_executable(CodeSimilarity main.cpp)

target_link_libraries(CodeSimilarity
        LLVM-9
        Graphgen
        Similarity
        )