set(SOURCE)

add_subdirectory(TextSimilarity)
add_subdirectory(GraphSimilarity)
add_subdirectory(utils)

add_library(Similarity Similarity.cpp)

target_link_libraries(Similarity
        GraphSimilarity
        TextSimilarity
        )