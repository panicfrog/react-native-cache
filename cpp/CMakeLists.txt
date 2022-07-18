cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "HCache")
set (BUILD_DIR ./build)

project(${PACKAGE_NAME})

include_directories(
	${PACKAGE_NAME}
	"."
	)
  
set(HCACHE_SRC
    ${PROJECT_SOURCE_DIR}/src/CacheValue.cpp
    ${PROJECT_SOURCE_DIR}/src/DiskStorage.cpp
)

set(HCACHE_INC
	${PROJECT_SOURCE_DIR}/include/CacheValue.hpp
	${PROJECT_SOURCE_DIR}/include/DiskStorage.hpp
)

include_directories(
	./include
	../thirdparty/SQLiteCpp/include
	)

if(${CMAKE_SYSTEM_NAME} MATCHES "iOS")
	add_library(
		${PACKAGE_NAME}
		STATIC
		${HCACHE_SRC}
		${HCACHE_INC}
		)
else()
	add_library(
		${PACKAGE_NAME}
		SHARED
    	${HCACHE_SRC}
    	${HCACHE_INC}
		)
endif()


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
