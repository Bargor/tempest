# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/engine.h
	engine/shader_compiler.h
	engine/scene/scene.h
	engine/OpenGL/opengl_exception.h
	engine/OpenGL/shader.h
)

set(ENGINE_SRCS 
	engine/engine.cpp
	engine/shader_compiler.cpp
	engine/scene/scene.cpp
	engine/OpenGL/shader.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application glm ${GLEW_LIBRARIES})

if(UNIX)
	target_link_libraries(engine PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)
