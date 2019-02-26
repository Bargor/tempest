# This file is part of Tempest project
# Author: Karol Kontny

set(BACKEND_HDRS
	backend/api_exception.h
	backend/shader.h
)

set(BACKEND_SRCS 
	backend/shader.cpp
)

add_definitions (-DGLEW_NO_GLU)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
find_package(GLEW REQUIRED)

tst_add_library(backend "${BACKEND_HDRS}" "${BACKEND_SRCS}")

target_include_directories(backend PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(backend PUBLIC tst::application glm glfw ${GLEW_LIBRARIES})

if(UNIX)
	target_link_libraries(backend PRIVATE -lGL -lX11 -lXxf86vm -lXrandr -lXi -ldl)
endif(UNIX)
