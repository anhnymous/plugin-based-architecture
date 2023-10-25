cmake_minimum_required(VERSION 3.21)

add_library(GIID_DB GIID_DB.cc)
target_link_libraries(GIID_DB PUBLIC Plugins FCoreLogger)
target_include_directories(GIID_DB PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(GIID_DB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})