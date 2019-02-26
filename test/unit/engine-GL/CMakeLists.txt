# This file is part of Tempest-engine-GL project
# Author: Karol Kontny

set(TEST_SRCS
	test_runner.cpp
	shader_compiler_test.cpp
	shader_test.cpp
)

source_group("engine GL tests" FILES ${TEST_SRCS})

add_definitions (-DGLEW_NO_GLU)

tst_add_executable(engine_test "" "${TEST_SRCS}")

add_test(NAME tst.engine_unit_test COMMAND engine_test)

target_link_libraries(engine_test PRIVATE tst::engine gmock gtest glfw)
