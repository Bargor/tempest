# This file is part of Tempest project
# Author: Karol Kontny

add_subdirectory(scene)
if (ENGINE STREQUAL "Engine-GL")
	add_subdirectory(engine-GL)
elseif(ENGINE STREQUAL "Engine-Vk")
	add_subdirectory(engine-Vk)
endif(ENGINE STREQUAL "Engine-GL")
