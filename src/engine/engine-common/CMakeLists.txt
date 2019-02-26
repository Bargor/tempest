# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/engine.h
	engine/shader_compiler.h
	scene/scene.h
)

set(ENGINE_SRCS 
	engine/engine.cpp
	engine/shader_compiler.cpp
	scene/scene.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_link_libraries(engine PUBLIC tst::backend tst::application glm)