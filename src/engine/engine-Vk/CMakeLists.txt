# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/instance.h
	engine/vulkan_exception.h
	engine/device.h
	engine/engine_init.h
	engine/engine.h
	engine/queue_indices.h
	engine/swap_chain.h
	engine/buffer.h
	engine/uniform_buffer.h
	engine/vertex_format.h
	engine/vertex_buffer.h
	engine/index_buffer.h
	engine/resources/shader.h
	engine/resources/shader_compiler.h
)

set(ENGINE_SRCS 
	engine/instance.cpp
	engine/device.cpp
	engine/engine_init.cpp
	engine/engine.cpp
	engine/queue_indices.cpp
	engine/swap_chain.cpp
	engine/buffer.cpp
	engine/uniform_buffer.cpp
	engine/vertex_format.cpp
	engine/vertex_buffer.cpp
	engine/index_buffer.cpp
	engine/resources/shader.cpp
	engine/resources/shader_compiler.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application glm glfw ${Vulkan_LIBRARIES})

add_custom_target(
        copy_shader_files
        ${CMAKE_COMMAND}
        -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/shaders/
        -P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
        COMMENT "Copying Files for target: test_SphereShader" VERBATIM
        )

add_dependencies(engine copy_shader_files )
