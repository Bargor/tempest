# This file is part of Tempest project
# Author: Karol Kontny

set(TEST_SRCS
	test_runner.cpp
    test_environment.cpp
    test_environment.h
    buffer_test.cpp
    index_buffer_test.cpp
    physical_device_test.cpp
    shader_compiler_test.cpp
    shader_test.cpp
    texture_test.cpp
    uniform_buffer_test.cpp
    vertex_buffer_test.cpp
)

source_group("Engine VK tests" FILES ${TEST_SRCS})

add_definitions (-DGLEW_NO_GLU)

tst_add_executable(engine_test "" "${TEST_SRCS}")

add_test(NAME tst.engine_unit_test COMMAND engine_test)

target_link_libraries(engine_test PRIVATE tst::engine-impl gmock gtest glfw)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_custom_target(
		copy_test_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test_shaders/
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/test_shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying shader files for target: test" VERBATIM
		)
else()
	add_custom_target(
		copy_test_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test_shaders/
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/test_shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying shader files for target: test" VERBATIM
		)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

add_dependencies(engine_test copy_test_shader_files)
