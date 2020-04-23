cmake_minimum_required(VERSION 3.12)
project(CodeSimilarity)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


if (${CMAKE_BUILD_TYPE} EQUAL DEBUG)
    set(DCMAKE_CXX_FLAGS ${DCMAKE_CXX_FLAGS} -pg)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -pg)
endif ()

include_directories(
        .
        ./include
        /usr/local/include/eigen3
)

link_directories(/usr/local/lib)

# Get LLVM library
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Add sub directory
add_subdirectory(Graphgen)
add_subdirectory(Similarity)
add_subdirectory(Admin)
add_subdirectory(Runner)

add_executable(CodeSimilarity main.cpp)

target_link_libraries(CodeSimilarity
        Admin
        )
