# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
	engine/opengl_exception.h
	engine/shader.h
	engine/shader_compiler.h
	engine/engine.h
)

set(ENGINE_SRCS 
	engine/shader.cpp
	engine/shader_compiler.cpp
	engine/engine.cpp
)

add_definitions (-DGLEW_NO_GLU)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
find_package(GLEW REQUIRED)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(engine PUBLIC tst::application tst::scene glm glfw PRIVATE ${GLEW_LIBRARIES})

if(UNIX)
	target_link_libraries(engine PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)
