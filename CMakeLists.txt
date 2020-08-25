# This file is part of Tempest project
# Author: Karol Kontny

cmake_minimum_required(VERSION 3.13)

project (Tempest CXX)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   6   CACHE STRING "Project patch version number.")

#Get build number
execute_process(COMMAND git -C ${CMAKE_SOURCE_DIR} rev-parse --short HEAD OUTPUT_VARIABLE VERSION_REVISION)

message(${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}:${VERSION_REVISION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	#General Win compile options
	message("Visual Studio compiler")
	set(TEMPEST_COMPILE_OPTIONS /W4 /WX /MP /std:c++latest)
	set(TEMPEST_COMPILE_DEFINITIONS NOMINMAX)
	set(DEBUG_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} /MDd)
	set(RELEASE_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} /Oi /MD /GL)
	set(RELWITHDEBINFO_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} /Oi /MD)
	#Win linker options
	set(TEMPEST_LINK_OPTIONS /WX)
	set(DEBUG_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
	set(RELEASE_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS} /LTCG)
	set(RELWITHDEBINFO_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS} /LTCG)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message("GCC compiler")
	set(TEMPEST_COMPILE_OPTIONS -Wall -Werror -W -Wextra -pedantic-errors)
	set(TEMPEST_COMPILE_DEFINITIONS FMT_USE_USER_DEFINED_LITERALS=0)
	set(DEBUG_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O0 -march=core2)
	set(RELEASE_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O3 -funsafe-loop-optimizations -m64 -march=core2 -mfpmath=sse)
	set(RELWITHDEBINFO_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O3 -funsafe-loop-optimizations -m64 -march=core2 -mfpmath=sse)
	#GCC link options
	set(TEMPEST_LINK_OPTIONS)
	set(DEBUG_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
	set(RELEASE_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
	set(RELWITHDEBINFO_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message("Clang compiler")
	set(TEMPEST_COMPILE_OPTIONS -Wall -Werror -W -Wextra -pedantic-errors)
	set(DEBUG_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O0 -march=core2)
	set(RELEASE_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O3 -m64 -march=core2 -mfpmath=sse)
	set(RELWITHDEBINFO_COMPILE_OPTIONS ${TEMPEST_COMPILE_OPTIONS} -O3 -m64 -march=core2 -mfpmath=sse)
	#Clang link options
	set(TEMPEST_LINK_OPTIONS)
	set(DEBUG_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
	set(RELEASE_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
	set(RELWITHDEBINFO_LINK_OPTIONS ${TEMPEST_LINK_OPTIONS})
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#General compiler options

set(RELWITHDEBINFO_COMPILE_DEFINITIONS ${TEMPEST_COMPILE_DEFINITIONS} NDEBUG)
set(RELEASE_COMPILE_DEFINITIONS ${TEMPEST_COMPILE_DEFINITIONS} NDEBUG)
set(DEBUG_COMPILE_DEFINITIONS ${TEMPEST_COMPILE_DEFINITIONS})

set(RUNTIME_OUTPUT_PATH ${CMAKE_BINARY_DIR}/exe)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)
set(ARCHIVE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/exe)

#Build lib
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "Build the GLFW example programs")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW test programs")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "Build the GLFW documentation")
	
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")

add_subdirectory(libs/fmt)
add_subdirectory(libs/rapidjson)
add_subdirectory(libs/tinyobjloader)
add_subdirectory(src)

if(TEMPEST_BUILD_ALL)
	set(TEMPEST_BUILD_UNIT_TESTS ON)
	set(TEMPEST_BUILD_PERF_TESTS ON)
endif(TEMPEST_BUILD_ALL)

#Build unit tests
if(TEMPEST_BUILD_UNIT_TESTS)
	include(CTest)
	enable_testing()
	
	if(WIN32)
		set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
	endif(WIN32)
	
	add_subdirectory(libs/googletest)	

	mark_as_advanced(
		BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
		gmock_build_tests gtest_build_samples gtest_build_tests
		gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
	)

	set_target_properties(gtest PROPERTIES FOLDER extern)
	set_target_properties(gtest_main PROPERTIES FOLDER extern)
	set_target_properties(gmock PROPERTIES FOLDER extern)
	set_target_properties(gmock_main PROPERTIES FOLDER extern)
	
	add_subdirectory(test/unit)
endif(TEMPEST_BUILD_UNIT_TESTS)

#Build perf tests
if(TEMPEST_BUILD_PERF_TESTS)
	set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Google benchmark tests")

	add_subdirectory(libs/benchmark)
	add_subdirectory(test/perf)
endif(TEMPEST_BUILD_PERF_TESTS)
