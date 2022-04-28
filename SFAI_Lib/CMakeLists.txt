cmake_minimum_required(VERSION 3.5)

find_package(frugally-deep CONFIG REQUIRED)
add_library(SFAI SHARED library.cpp)
target_link_libraries(SFAI PRIVATE frugally-deep::fdeep)
target_include_directories(SFAI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SFAI PRIVATE ${frugally-deep_INCLUDE_DIRS})