include_directories("${PROJECT_SOURCE_DIR}")

add_executable(puristin_test test_main.cpp)
target_link_libraries(puristin_test gtest gmock_main)
add_test(NAME puristin_test COMMAND puristin_test)
