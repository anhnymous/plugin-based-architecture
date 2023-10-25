cmake_minimum_required(VERSION 3.21)

add_library(Main Application.cc VersionHandler.cc HelpHandler.cc MainHandler.cc)
target_include_directories(Main PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(Main INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Main PUBLIC FCoreLogger ConfManager
                                  ExitManager SignalManager GIID_DB grpcServer)
