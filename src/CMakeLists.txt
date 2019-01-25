set(LIB_SRC
  file_analyzer.cpp)

add_library(puristinlib ${LIB_SRC})

add_executable(puristin main.cpp)
target_link_libraries(puristin puristinlib)
