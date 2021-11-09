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
)

target_include_directories(tester
	PRIVATE
	${PROJECT_SOURCE_DIR}/MTF
	${PROJECT_SOURCE_DIR}/archiver
	${PROJECT_SOURCE_DIR}/BWT
	${PROJECT_SOURCE_DIR}/tree/header)

target_link_libraries(tester archiver-lib bwt huffman-tree mtf)
target_link_libraries(tester -lpthread)
