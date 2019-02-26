# This file is part of Tempest project
# Author: Karol Kontny

if(TEMPEST_BUILD_ALL)
	add_subdirectory(core)
	
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	
	set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")

	add_definitions (-DGLEW_NO_GLU)
	if (ENGINE STREQUAL "Engine-Vk")
		add_definitions (-DGLFW_INCLUDE_VULKAN)
	endif(ENGINE STREQUAL "Engine-Vk")
	
	list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/libs/glew/")
	find_package(GLEW REQUIRED)
	
	add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw ${LIBRARY_OUTPUT_PATH}/glfw)
	add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glm ${LIBRARY_OUTPUT_PATH}/glm)
	
	add_subdirectory(device)
	add_subdirectory(application)
	add_subdirectory(engine)
	add_subdirectory(tempest)
endif(TEMPEST_BUILD_ALL)