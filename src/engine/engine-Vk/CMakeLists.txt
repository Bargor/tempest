# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/vulkan_exception.h
	engine/engine_init.h
	engine/engine.h
	engine/queue_indices.h
	engine/swap_chain.h
	engine/device_queue.h
	engine/resources/shader.h
	engine/resources/shader_compiler.h
)

set(ENGINE_SRCS 
	engine/engine.cpp
	engine/engine_init.cpp
	engine/queue_indices.cpp
	engine/swap_chain.cpp
	engine/device_queue.cpp
	engine/resources/shader.cpp
	engine/resources/shader_compiler.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application tst::scene glm glfw ${Vulkan_LIBRARIES})

add_custom_target(
        copy_shader_files
        ${CMAKE_COMMAND}
        -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -D DESTINATION_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${PROJECT_SOURCE_DIR}/cmake/CopyShaders.cmake
        COMMENT "Copying Files for target: test_SphereShader" VERBATIM
        )

add_dependencies(engine copy_shader_files )
