# This file is part of Tempest project
# Author: Karol Kontny

add_subdirectory(resources)
add_subdirectory(scene)
add_subdirectory(engine-common)
if (ENGINE STREQUAL "GL")
	add_subdirectory(engine-GL)
elseif(ENGINE STREQUAL "Vk")
	add_subdirectory(engine-Vk)
endif(ENGINE STREQUAL "GL")
