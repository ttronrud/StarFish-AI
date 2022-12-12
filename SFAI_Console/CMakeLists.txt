cmake_minimum_required(VERSION 3.5)

add_executable(SFAIC SFAI_Console.cpp)
target_link_libraries(SFAIC SFAI)

add_custom_command(TARGET SFAIC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:SFAI>
        $<TARGET_FILE_DIR:SFAIC>)

add_custom_command(TARGET SFAIC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/test_dat_good.txt
        ${CMAKE_SOURCE_DIR}/fdeep_model.json
        $<TARGET_FILE_DIR:SFAIC>)
