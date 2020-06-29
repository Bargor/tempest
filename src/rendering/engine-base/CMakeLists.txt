# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_BASE_HDRS
	engine-base/gpu_info.h
	engine-base/rendering_technique.h
	engine-base/material.h
	engine-base/pipeline_parser.h
	engine-base/pipeline_settings.h
    engine-base/resource_bind_point.h
	engine-base/settings.h
	engine-base/shader.h
	engine-base/settings_parser.h
	engine-base/technique_parser.h
    engine-base/texture.h
    engine-base/uniform_buffer.h
	engine-base/vertex_format.h
)

set(ENGINE_BASE_SRCS
	engine-base/gpu_info.cpp
	engine-base/rendering_technique.cpp
	engine-base/material.cpp
	engine-base/pipeline_parser.cpp
	engine-base/pipeline_settings.cpp
	engine-base/settings.cpp
	engine-base/settings_parser.cpp
	engine-base/technique_parser.cpp
	engine-base/vertex_format.cpp
)

tst_add_library(engine-base "${ENGINE_BASE_HDRS}" "${ENGINE_BASE_SRCS}")

target_link_libraries(engine-base PUBLIC glm tst::core rapidjson::rapidjson PRIVATE tst::application fmt::fmt)
