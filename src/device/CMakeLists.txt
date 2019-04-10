# This file is part of Tempest project
# Author: Karol Kontny

set(DEVICE_HDRS
	device/monitor.h
)

set(DEVICE_SRCS 
	device/monitor.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(device "${DEVICE_HDRS}" "${DEVICE_SRCS}")

target_link_libraries(device PUBLIC tst::core PRIVATE glfw)

if (ENGINE STREQUAL "Vk")
	target_include_directories(device PRIVATE ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(device PRIVATE ${Vulkan_LIBRARIES})
endif (ENGINE STREQUAL "Vk")

if(UNIX)
	target_link_libraries(device PRIVATE -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)