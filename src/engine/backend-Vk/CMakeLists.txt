# This file is part of Tempest project
# Author: Karol Kontny

set(BACKEND_HDRS
	backend/init.h
)

set(BACKEND_SRCS 
	backend/init.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

find_package(Vulkan REQUIRED)

tst_add_library(backend "${BACKEND_HDRS}" "${BACKEND_SRCS}")

target_include_directories(backend PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(backend PUBLIC tst::application glm glfw Vulkan_LIBRARIES)
