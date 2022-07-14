cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "HCache")
set (BUILD_DIR ./build)

project(${PACKAGE_NAME})

include_directories(
	${PACKAGE_NAME}
	"."
	)

add_library(
	${PACKAGE_NAME}
	STATIC
	CacheValue.cpp
	CacheValue.hpp
	DiskStorage.cpp
	DiskStorage.hpp
	)

add_subdirectory(../thirdparty/SQLiteCpp SQLiteCppOutDir)

set_target_properties(
	${PACKAGE_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	POSITON_INDEPENDENT ON
	)

target_link_libraries(
	${PACKAGE_NAME}
	SQLiteCpp
	)
