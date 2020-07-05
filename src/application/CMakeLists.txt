# This file is part of Tempest project
# Author: Karol Kontny

set(APPLICATION_HDRS
    application/app_event.h
    application/argument_parser.h
	application/data_exception.h
	application/data_loader.h
	application/event_processor.h
	application/glfw_window.h
	application/glfw_exception.h
	application/image_data.h
	application/input_processor.h
	application/main_window.h
	application/object_id.h
	application/time_source.h
	application/window.h
)

set(APPLICATION_SRCS 
	application/app_event.cpp
	application/argument_parser.cpp
	application/data_loader.cpp
	application/event_processor.cpp	
	application/glfw_window.cpp	
	application/input_processor.cpp	
	application/main_window.cpp
	application/time_source.cpp
	application/window.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(application "${APPLICATION_HDRS}" "${APPLICATION_SRCS}")

target_include_directories(application PRIVATE ${PROJECT_SOURCE_DIR}/libs/stb)

target_link_libraries(application PUBLIC tst::core tst::device rapidjson::rapidjson PRIVATE glfw)

if (ENGINE STREQUAL "Vk")
	target_include_directories(application PRIVATE ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(application PRIVATE ${Vulkan_LIBRARIES})
endif (ENGINE STREQUAL "Vk")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(application PRIVATE -lstdc++fs)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_link_libraries(application PRIVATE -lc++fs)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")