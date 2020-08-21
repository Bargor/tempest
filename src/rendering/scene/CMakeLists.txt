# This file is part of Tempest project
# Author: Karol Kontny

set(SCENE_HDRS
	scene/camera.h
    scene/obj_loader.h
	scene/object_controller.h
	scene/scene.h
	scene/scene_object.h
)

set(SCENE_SRCS 
	scene/camera.cpp
    scene/obj_loader.cpp
	scene/object_controller.cpp
	scene/scene.cpp
	scene/scene_object.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(scene "${SCENE_HDRS}" "${SCENE_SRCS}")

target_link_libraries(scene PUBLIC tst::engine tst::materials PRIVATE tinyobjloader)
