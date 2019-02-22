# This file is part of Tempest project
# Author: Karol Kontny

set(TEMPEST_HDRS
	tempest/application.h
	tempest/engine.h
)

set(TEMPEST_SRCS 
	tempest/application.cpp
	tempest/engine.cpp
	tempest/main.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_executable(tempest "${TEMPEST_HDRS}" "${TEMPEST_SRCS}")

target_link_libraries(tempest PUBLIC tst::application tst::engine tst::device glfw)
