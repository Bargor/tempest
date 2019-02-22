# This file is part of Tempest project
# Author: Karol Kontny

add_subdirectory(core)
if(ENGINE_GL)
	add_subdirectory(engine-GL)
endif(ENGINE_GL)