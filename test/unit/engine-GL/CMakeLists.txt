# This file is part of Tempest-engine-GL project
# Author: Karol Kontny

enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/googletest ${LIBRARY_OUTPUT_PATH})


set(TEST_SRCS
	test_runner.cpp
	data_loader_test.cpp
	shader_compiler_test.cpp
	shader_test.cpp
)

tst_add_executable (engine_test "" "${TEST_SRCS}")

add_test(NAME tst.engine_unit_test COMMAND engine_test)

target_link_libraries(engine_test tst::engine gmock gtest glfw ${GLEW_LIBRARIES})

if(UNIX)
	target_link_libraries(engine_test ${CMAKE_THREAD_LIBS_INIT})
endif(UNIX)

