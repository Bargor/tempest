# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/init.h
)

set(ENGINE_SRCS 
	engine/init.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${BACKEND_HDRS}" "${BACKEND_SRCS}")

target_include_directories(engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application tst::scene glm glfw ${Vulkan_LIBRARIES})
