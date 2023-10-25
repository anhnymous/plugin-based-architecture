cmake_minimum_required(VERSION 3.21)

add_library(Services Read.cc Write.cc)
target_link_libraries(Services PUBLIC grpcServiceRead grpcServiceWrite)
target_include_directories(Services PRIVATE ${PROJECT_BINARY_DIR})
target_include_directories(Services INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
