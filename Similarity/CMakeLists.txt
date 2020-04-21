set(SOURCE Similarity.cpp)

add_subdirectory(TextSimilarity)
add_subdirectory(GraphSimilarity)
add_subdirectory(utils)

add_library(Similarity ${SOURCE})

target_link_libraries(Similarity
        GraphSimilarity
        TextSimilarity
        )