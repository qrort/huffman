add_library(archiver-lib
	archiver.cpp
)

target_include_directories(archiver-lib
			PUBLIC
			${CMAKE_CURRENT_DIR}
			${PROJECT_SOURCE_DIR}/MTF
			${PROJECT_SOURCE_DIR}/BWT
			${PROJECT_SOURCE_DIR}/tree/header)

target_link_libraries(archiver-lib PRIVATE huffman-tree bwt mtf)

add_executable(archiver-exe
	main.cpp
)

target_include_directories(archiver-exe
			PRIVATE
			${CMAKE_CURRENT_DIR}
			${PROJECT_SOURCE_DIR}/MTF
			${PROJECT_SOURCE_DIR}/BWT
			${PROJECT_SOURCE_DIR}/tree/header)

target_link_libraries(archiver-exe PRIVATE archiver-lib huffman-tree bwt mtf)
