# This file is part of Tempest project
# Author: Karol Kontny

set(SCENE_HDRS
	scene/scene.h
)

set(SCENE_SRCS 
	scene/scene.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(scene "${SCENE_HDRS}" "${SCENE_SRCS}")

target_link_libraries(scene PUBLIC tst::application glm)