include_directories("${PROJECT_SOURCE_DIR}")

set(TEST_SRC
  test.cpp
  test_file_analyzer.cpp)

add_executable(puristin_test ${TEST_SRC})
target_link_libraries(puristin_test gtest gmock_main)
target_link_libraries(puristin_test puristinlib)

add_test(NAME puristin_test COMMAND puristin_test)
