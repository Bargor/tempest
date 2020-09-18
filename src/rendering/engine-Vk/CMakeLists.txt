# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine-impl/api.h
	engine-impl/device.h
	engine-impl/draw_info.h
	engine-impl/engine_frontend.h
	engine-impl/gpu_info.h
	engine-impl/instance.h
	engine-impl/queue_indices.h
	engine-impl/physical_device.h
	engine-impl/resource_cache.h
	engine-impl/resource_factory.h
	engine-impl/resources/buffer.h
	engine-impl/resources/image.h
	engine-impl/resources/index_buffer.h
    engine-impl/resources/material.h
	engine-impl/resources/pipeline.h
	engine-impl/resources/rendering_technique.h
	engine-impl/resources/settings.h
	engine-impl/resources/shader.h
	engine-impl/resources/texture.h
	engine-impl/resources/uniform_buffer.h
	engine-impl/resources/vertex_buffer.h
	engine-impl/resources/vertex_format.h
	engine-impl/resources/util.h
	engine-impl/settings_parser.h
	engine-impl/shader_compiler.h
    engine-impl/surface_support.h
	engine-impl/swap_chain.h
	engine-impl/vulkan_exception.h
)

set(ENGINE_SRCS 
	engine-impl/device.cpp
	engine-impl/engine_frontend.cpp
	engine-impl/draw_info.cpp
	engine-impl/gpu_info.cpp
	engine-impl/instance.cpp
	engine-impl/queue_indices.cpp
	engine-impl/physical_device.cpp
	engine-impl/resource_cache.cpp
	engine-impl/resource_factory.cpp
	engine-impl/resources/buffer.cpp
	engine-impl/resources/image.cpp
	engine-impl/resources/index_buffer.cpp
    engine-impl/resources/material.cpp
	engine-impl/resources/pipeline.cpp
	engine-impl/resources/rendering_technique.cpp
	engine-impl/resources/settings.cpp
	engine-impl/resources/shader.cpp
	engine-impl/resources/texture.cpp
	engine-impl/resources/uniform_buffer.cpp
	engine-impl/resources/vertex_buffer.cpp
	engine-impl/resources/vertex_format.cpp
	engine-impl/resources/util.cpp
	engine-impl/settings_parser.cpp
	engine-impl/shader_compiler.cpp
	engine-impl/swap_chain.cpp
)

tst_add_library(engine-impl "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine-impl PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine-impl PUBLIC
	tst::application
	tst::engine-base
	glfw
	${Vulkan_LIBRARIES}
	PRIVATE
	tinyobjloader
	fmt::fmt
)

#Compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_custom_target(
		copy_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying shader files for target: test" VERBATIM
		)
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
		copy_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying shader files for target: test" VERBATIM
		)
	add_custom_target(
		copy_settings_file
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopySettings.cmake
		COMMENT "Copying settings for target: test" VERBATIM
		)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

add_dependencies(engine-impl copy_shader_files copy_settings_file)
