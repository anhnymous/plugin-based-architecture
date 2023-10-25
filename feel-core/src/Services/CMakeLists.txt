cmake_minimum_required(VERSION 3.21)

add_library(Services ReadServiceImpl.cc WriteServiceImpl.cc)
target_include_directories(Services INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Services PUBLIC grpcServiceRead grpcServiceWrite
                                      FCoreLogger GIID_DB)
