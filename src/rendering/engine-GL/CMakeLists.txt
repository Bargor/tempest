# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine-impl/api.h
	engine-impl/command_list.h
	engine-impl/device.h
	engine-impl/draw_info.h
	engine-impl/engine.h
	engine-impl/gpu_info.h
	engine-impl/opengl_exception.h
	engine-impl/resources/buffer.h
	engine-impl/resources/index_buffer.h
	engine-impl/resources/material.h
	engine-impl/resources/pipeline.h
	engine-impl/resources/settings.h
	engine-impl/resources/shader.h
	engine-impl/resources/uniform_buffer.h
	engine-impl/resources/vertex_buffer.h
	engine-impl/resources/vertex_format.h
	engine-impl/resource_factory.h
	engine-impl/settings_parser.h
	engine-impl/shader_compiler.h
)

set(ENGINE_SRCS 
    engine-impl/command_list.cpp
    engine-impl/device.cpp
	engine-impl/draw_info.cpp
	engine-impl/engine.cpp
	engine-impl/gpu_info.cpp
	engine-impl/resources/buffer.cpp
	engine-impl/resources/index_buffer.cpp
	engine-impl/resources/material.cpp
	engine-impl/resources/pipeline.cpp
	engine-impl/resources/settings.cpp
	engine-impl/resources/shader.cpp
	engine-impl/resources/uniform_buffer.cpp
	engine-impl/resources/vertex_buffer.cpp
	engine-impl/resources/vertex_format.cpp
	engine-impl/resource_factory.cpp
	engine-impl/settings_parser.cpp
	engine-impl/shader_compiler.cpp
)

add_definitions (-DGLEW_NO_GLU)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
find_package(GLEW REQUIRED)

tst_add_library(engine-impl "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine-impl PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(engine-impl PUBLIC tst::application tst::engine-base glfw PRIVATE ${GLEW_LIBRARIES})
if(WIN32)
    target_link_libraries(engine-impl PRIVATE opengl32)
endif(WIN32)
if(UNIX)
	target_link_libraries(engine-impl PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)

#Compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_custom_target(
		copy_settings_file
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopySettings.cmake
		COMMENT "Copying settings for target: test" VERBATIM
		)
else()
	add_custom_target(
		copy_settings_file
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopySettings.cmake
		COMMENT "Copying settings for target: test" VERBATIM
		)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

add_dependencies(engine-impl copy_settings_file)
