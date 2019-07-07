# This file is part of Tempest project
# Author: Karol Kontny

set(RESOURCES_HDRS
	resources/vertex_buffer.h
	resources/vertex_format.h
	resources/index_buffer.h
	resources/uniform_buffer.h
)

set(RESOURCES_SRCS 
	resources/vertex_buffer.cpp
	resources/vertex_format.cpp
	resources/index_buffer.cpp
	resources/uniform_buffer.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(resources "${RESOURCES_HDRS}" "${RESOURCES_SRCS}")

target_link_libraries(resources PUBLIC tst::engine glm)