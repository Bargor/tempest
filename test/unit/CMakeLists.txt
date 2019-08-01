# This file is part of Tempest project
# Author: Karol Kontny

add_subdirectory(core)
if (ENGINE STREQUAL "GL")
	add_subdirectory(engine-GL)
endif (ENGINE STREQUAL "GL")