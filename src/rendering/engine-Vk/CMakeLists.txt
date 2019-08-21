# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine-impl/api.h
	engine-impl/device.h
	engine-impl/draw_info.h
	engine-impl/engine.h
	engine-impl/engine_init.h
	engine-impl/gpu_info.h
	engine-impl/instance.h
	engine-impl/queue_indices.h
	engine-impl/physical_device.h
	engine-impl/swap_chain.h
	engine-impl/shader_compiler.h
	engine-impl/resource_factory.h
	engine-impl/resources/buffer.h
	engine-impl/resources/index_buffer.h
	engine-impl/resources/pipeline.h
	engine-impl/resources/shader.h
	engine-impl/resources/uniform_buffer.h
	engine-impl/resources/vertex_buffer.h
	engine-impl/resources/vertex_format.h
	engine-impl/vulkan_exception.h
)

set(ENGINE_SRCS 
	engine-impl/device.cpp
	engine-impl/engine.cpp
	engine-impl/draw_info.cpp
	engine-impl/engine_init.cpp
	engine-impl/gpu_info.cpp
	engine-impl/instance.cpp
	engine-impl/queue_indices.cpp
	engine-impl/physical_device.cpp
	engine-impl/swap_chain.cpp
	engine-impl/shader_compiler.cpp
	engine-impl/resource_factory.cpp
	engine-impl/resources/buffer.cpp
	engine-impl/resources/index_buffer.cpp
	engine-impl/resources/pipeline.cpp
	engine-impl/resources/shader.cpp
	engine-impl/resources/uniform_buffer.cpp
	engine-impl/resources/vertex_buffer.cpp
	engine-impl/resources/vertex_format.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine-impl "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine-impl PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine-impl PUBLIC tst::application tst::engine-base glfw ${Vulkan_LIBRARIES})

#Compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_custom_target(
		copy_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying Files for target: test_SphereShader" VERBATIM
		)
else()
	add_custom_target(
		copy_shader_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/shaders/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
		COMMENT "Copying Files for target: test_SphereShader" VERBATIM
		)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")


add_dependencies(engine-impl copy_shader_files)
