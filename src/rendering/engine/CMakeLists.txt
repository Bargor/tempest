# This file is part of Tempest project
# Author: Karol Kontny

set(ENGINE_HDRS
    engine/device.h
	engine/draw_info.h
	engine/gpu_info.h
    engine/material.h
    engine/mesh.h
    engine/model.h
	engine/resource_factory.h
    engine/resources/index_buffer.h
	engine/resources/pipeline.h
	engine/resources/texture.h
	engine/resources/uniform_buffer.h
	engine/resources/vertex_buffer.h
	engine/resources/vertex_format.h
    engine/resources/view.h
	engine/settings.h
)

set(ENGINE_SRCS 
    engine/device.cpp
	engine/draw_info.cpp
	engine/gpu_info.cpp
    engine/material.cpp
    engine/mesh.cpp
    engine/model.cpp
	engine/resource_factory.cpp
	engine/resources/index_buffer.cpp
	engine/resources/pipeline.cpp
	engine/resources/texture.cpp
	engine/resources/uniform_buffer.cpp
	engine/resources/vertex_buffer.cpp
	engine/resources/vertex_format.cpp
    engine/resources/view.cpp
	engine/settings.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(engine "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_link_libraries(engine PUBLIC tst::engine-impl)

#Compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_custom_target(
		copy_resource_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/$<CONFIG>/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyResources.cmake
		COMMENT "Copying resource files for target: test" VERBATIM
	)
else()
	add_custom_target(
		copy_resource_files
		${CMAKE_COMMAND}
		-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
		-D DESTINATION_DIR=${RUNTIME_OUTPUT_PATH}/
		-P ${PROJECT_SOURCE_DIR}/cmake/CopyResources.cmake
		COMMENT "Copying resource files for target: test" VERBATIM
	)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

add_dependencies(engine-base copy_resource_files)
