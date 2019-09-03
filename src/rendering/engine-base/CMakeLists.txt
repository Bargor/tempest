# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_BASE_HDRS
	engine-base/gpu_info.h
	engine-base/rendering_technique.h
	engine-base/resources/material.h
	engine-base/resources/pipeline.h
	engine-base/resources/vertex_format.h
)

set(ENGINE_BASE_SRCS
	engine-base/gpu_info.cpp
	engine-base/rendering_technique.cpp
	engine-base/resources/material.cpp
	engine-base/resources/pipeline.cpp
	engine-base/resources/vertex_format.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine-base "${ENGINE_BASE_HDRS}" "${ENGINE_BASE_SRCS}")

target_link_libraries(engine-base PUBLIC fmt::fmt glm tst::core)
