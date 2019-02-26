# This file is part of Tempest project
# Author: Karol Kontny

set(BACKEND_HDRS
)

set(BACKEND_SRCS 
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(backend "${ENGINE_HDRS}" "${ENGINE_SRCS}")

target_link_libraries(backend PUBLIC tst::application glm glfw)
