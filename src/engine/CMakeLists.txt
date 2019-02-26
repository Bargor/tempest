# This file is part of Tempest project
# Author: Karol Kontny

if (ENGINE STREQUAL "Engine-GL")
	add_subdirectory(backend-GL)
elseif(ENGINE STREQUAL "Engine-Vk")
	add_subdirectory(backend-Vk)
endif(ENGINE STREQUAL "Engine-GL")
add_subdirectory(engine-common)
