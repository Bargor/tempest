# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine/api.h
	engine/instance.h
	engine/vulkan_exception.h
	engine/device.h
	engine/engine_init.h
	engine/engine.h
	engine/engine_frontend.h
	engine/queue_indices.h
	engine/swap_chain.h
	engine/shader_compiler.h
	engine/resources/buffer.h
	engine/resources/uniform_buffer.h
	engine/resources/vertex_format.h
	engine/resources/vertex_buffer.h
	engine/resources/shader.h
	engine/resources/index_buffer.h
)

set(ENGINE_SRCS 
    engine/api.cpp
	engine/instance.cpp
	engine/device.cpp
	engine/engine_init.cpp
	engine/engine.cpp
	engine/engine_frontend.cpp
	engine/queue_indices.cpp
	engine/swap_chain.cpp
	engine/shader_compiler.cpp
	engine/resources/buffer.cpp
	engine/resources/uniform_buffer.cpp
	engine/resources/vertex_format.cpp
	engine/resources/vertex_buffer.cpp
	engine/resources/shader.cpp
	engine/resources/index_buffer.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application tst::engine-common glfw ${Vulkan_LIBRARIES})

add_custom_target(
        copy_shader_files
        ${CMAKE_COMMAND}
        -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/shaders/
        -P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
        COMMENT "Copying Files for target: test_SphereShader" VERBATIM
        )

add_dependencies(engine copy_shader_files)
