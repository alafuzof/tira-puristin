include_directories("${PROJECT_SOURCE_DIR}")

set(TEST_SRC
  test.cpp
  test_file_analyzer.cpp)

add_executable(puristin_test ${TEST_SRC})
target_link_libraries(puristin_test gtest gmock_main)
target_link_libraries(puristin_test puristinlib)

if(${CMAKE_VERSION} VERSION_LESS "3.9.0")
  add_test(NAME puristin_test COMMAND puristin_test)
else()
  include(GoogleTest)
  if(${CMAKE_VERSION} VERSION_EQUAL "3.9.0")
    gtest_add_tests(TARGET puristin_test)
  else()
    gtest_discover_tests(puristin_test)
  endif()
endif()