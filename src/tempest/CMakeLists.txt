# This file is part of Tempest project
# Author: Karol Kontny

set(TEMPEST_HDRS
	tempest/application.h
	tempest/engine.h
	tempest/context.h
)

set(TEMPEST_SRCS 
	tempest/application.cpp
	tempest/engine.cpp
	tempest/main.cpp
)

if (ENGINE STREQUAL "GL")
	list(APPEND TEMPEST_SRCS tempest/context_gl.cpp)
elseif (ENGINE STREQUAL "Vk")
	list(APPEND TEMPEST_SRCS tempest/context_vk.cpp)
endif (ENGINE STREQUAL "GL")

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_executable(tempest "${TEMPEST_HDRS}" "${TEMPEST_SRCS}")

target_link_libraries(tempest PUBLIC tst::application tst::engine tst::scene tst::device glfw)
