# This file is part of Tempest-core project
# Author: Karol Kontny

set(TEST_SRCS 
    test_runner.cpp
    algorithm/algorithm_test.cpp
	container/queue_test.cpp
	memory/frame_allocator_test.cpp
	numeric/random_test.cpp
	time/timer_test.cpp
	util/bitpack_test.cpp
)

source_group("core tests" FILES ${TEST_SRCS})

tst_add_executable (core_test "" "${TEST_SRCS}")
add_test(NAME tst.core_unit_test COMMAND core_test)

target_link_libraries(core_test PRIVATE tst::core gmock)

if(UNIX)
	target_link_libraries(core_test PRIVATE gtest)
endif(UNIX)
