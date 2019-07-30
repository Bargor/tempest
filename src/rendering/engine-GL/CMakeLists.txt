# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine-impl/api.h
	engine-impl/device.h
	engine-impl/draw_info.h
	engine-impl/engine.h
	engine-impl/opengl_exception.h
	engine-impl/resources/index_buffer.h
	engine-impl/resources/uniform_buffer.h
	engine-impl/resources/vertex_buffer.h
	engine-impl/resources/vertex_format.h
	engine-impl/resource_factory.h
	engine-impl/shader.h
	engine-impl/shader_compiler.h
)

set(ENGINE_SRCS 
    engine-impl/device.cpp
	engine-impl/draw_info.cpp
	engine-impl/engine.cpp
	engine-impl/resources/index_buffer.cpp
	engine-impl/resources/uniform_buffer.cpp
	engine-impl/resources/vertex_buffer.cpp
	engine-impl/resources/vertex_format.cpp
	engine-impl/resource_factory.cpp
	engine-impl/shader.cpp
	engine-impl/shader_compiler.cpp
)

add_definitions (-DGLEW_NO_GLU)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
find_package(GLEW REQUIRED)

tst_add_library(engine-impl "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_include_directories(engine-impl PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(engine-impl PUBLIC tst::application tst::engine-base glfw PRIVATE ${GLEW_LIBRARIES})

if(UNIX)
	target_link_libraries(engine-impl PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)
