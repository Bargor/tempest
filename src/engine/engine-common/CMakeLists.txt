# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_COMMON_HDRS
	engine/resources/vertex_format.h
)

set(ENGINE_COMMON_SRCS 
	engine/resources/vertex_format.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine-common "${ENGINE_COMMON_HDRS}" "${ENGINE_COMMON_SRCS}")

target_link_libraries(engine-common PUBLIC glm fmt::fmt)
