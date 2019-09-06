# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_BASE_HDRS
	engine-base/gpu_info.h
	engine-base/rendering_technique.h
	engine-base/material.h
	engine-base/pipeline.h
	engine-base/vertex_format.h
	engine-base/settings.h
)

set(ENGINE_BASE_SRCS
	engine-base/gpu_info.cpp
	engine-base/rendering_technique.cpp
	engine-base/material.cpp
	engine-base/pipeline.cpp
	engine-base/vertex_format.cpp
	engine-base/settings.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine-base "${ENGINE_BASE_HDRS}" "${ENGINE_BASE_SRCS}")

target_link_libraries(engine-base PUBLIC fmt::fmt glm tst::core)
