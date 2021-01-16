# This file is part of Tempest project
# Author: Karol Kontny

set(TEST_SRCS
	test_runner.cpp
    buffer_test.cpp
)

source_group("Engine VK tests" FILES ${TEST_SRCS})

add_definitions (-DGLEW_NO_GLU)

tst_add_executable(engine_test "" "${TEST_SRCS}")

add_test(NAME tst.engine_unit_test COMMAND engine_test)

target_link_libraries(engine_test PRIVATE tst::engine-impl gmock gtest glfw)
