# This file is part of Tempest-engine-GL project
# Author: Karol Kontny

set(TEST_SRCS
	test_runner.cpp
	shader_compiler_test.cpp
	shader_test.cpp
)

source_group("engine GL tests" FILES ${TEST_SRCS})

add_definitions (-DGLEW_NO_GLU)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
find_package(GLEW REQUIRED)

tst_add_executable(engine_test "" "${TEST_SRCS}")

target_include_directories(engine_test PUBLIC ${GLEW_INCLUDE_DIRS})

add_test(NAME tst.engine_unit_test COMMAND engine_test)

target_link_libraries(engine_test PRIVATE
    tst::engine-impl
    gmock
    gtest
    GLEW::GLEW
    ${GLEW_LIBRARIES}
)

if(WIN32)
    target_link_libraries(engine-impl PRIVATE opengl32)
endif(WIN32)
if(UNIX)
	target_link_libraries(engine-impl PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)