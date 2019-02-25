include_directories("${PROJECT_SOURCE_DIR}")

set(TEST_SRC
  test.cpp
  test_binary_tree.cpp
  test_bit_reader.cpp
  test_bit_writer.cpp
  test_file_analyzer.cpp
  test_huffman_code.cpp
  test_lzw_code.cpp
  test_lzw_dictionary.cpp
  test_priority_queue.cpp)

add_executable(puristin_test ${TEST_SRC})
target_link_libraries(puristin_test gtest gmock_main)
target_link_libraries(puristin_test puristinlib)

if(BUILD_PERF)
  set(PERF_SRC
    test.cpp
    test_performance.cpp)

  add_executable(puristin_perf ${PERF_SRC})
  target_link_libraries(puristin_perf gtest gmock_main)
  target_link_libraries(puristin_perf puristinlib)

  message("Downloading and extracting test data (Silesia corpus)")
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test_data)
  file(DOWNLOAD http://sun.aei.polsl.pl/~sdeor/corpus/silesia.zip ${PROJECT_BINARY_DIR}/test_data/silesia.zip SHOW_PROGRESS)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz silesia.zip
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/test_data)

endif()

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
