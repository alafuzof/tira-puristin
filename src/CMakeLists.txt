set(LIB_SRC
  abstract_code.cpp
  bit_reader.cpp
  bit_writer.cpp
  cli.cpp
  file_analyzer.cpp
  huffman_code.cpp
  lzw_code.cpp
  lzw_dictionary.cpp)

add_library(puristinlib ${LIB_SRC})

add_executable(puristin main.cpp)
target_link_libraries(puristin puristinlib)
