# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine/device.h
	engine/draw_info.h
	engine/engine.h
	engine/resource_factory.h
    engine/resources/index_buffer.h
	engine/resources/uniform_buffer.h
	engine/resources/vertex_buffer.h
	engine/resources/vertex_format.h
)

set(ENGINE_SRCS 
    engine/device.cpp
	engine/draw_info.cpp
	engine/engine.cpp
	engine/resource_factory.cpp
	engine/resources/index_buffer.cpp
	engine/resources/uniform_buffer.cpp
	engine/resources/vertex_buffer.cpp
	engine/resources/vertex_format.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_link_libraries(engine PUBLIC tst::engine-impl)
