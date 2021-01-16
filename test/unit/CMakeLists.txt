# This file is part of Tempest project
# Author: Karol Kontny

add_subdirectory(core)
if (ENGINE STREQUAL "GL")
	add_subdirectory(engine-GL)
elseif(ENGINE STREQUAL "Vk")
	add_subdirectory(engine-Vk)
endif(ENGINE STREQUAL "GL")