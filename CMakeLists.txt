cmake_minimum_required(VERSION 3.5)

project(ARCHIVER)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -D_GLIBCXX_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -D_GLIBCXX_RELEASE")
endif()

add_subdirectory(MTF)
add_subdirectory(tree)
add_subdirectory(BWT)
add_subdirectory(archiver)
