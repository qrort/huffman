cmake_minimum_required(VERSION 3.5)

project(HUFFMAN)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -D_GLIBCXX_RELEASE")
endif()

add_subdirectory(tree)
add_subdirectory(archiver)
