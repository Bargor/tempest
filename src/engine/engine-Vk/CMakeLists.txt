# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/vulkan_exception.h
	engine/engine_init.h
	engine/engine.h
	engine/queue_indices.h
	engine/swap_chain.h
	engine/device_queue.h
)

set(ENGINE_SRCS 
	engine/engine.cpp
	engine/engine_init.cpp
	engine/queue_indices.cpp
	engine/swap_chain.cpp
	engine/device_queue.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application tst::scene glm glfw ${Vulkan_LIBRARIES})
