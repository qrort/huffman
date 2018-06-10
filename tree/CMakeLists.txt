add_library(huffman-tree
	    implementation/tree-impl.cpp
)

target_include_directories(huffman-tree
			PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/header
			PRIVATE
			implementation
)

add_executable(tester
               testing/tester.cpp
               testing/gtest/gtest-all.cc
               testing/gtest/gtest.h
               testing/gtest/gtest_main.cc
	       header/tree-impl.h
	       implementation/tree-impl.cpp
)

target_link_libraries(tester -lpthread)
