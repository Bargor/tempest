# This file is part of Tempest project
# Author: Karol Kontny

set(MATERIALS_HDRS
    materials/test.h
)

set(MATERIALS_SRCS 
    materials/test.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/TstAddLibrary.cmake)

tst_add_library(materials "${MATERIALS_HDRS}" "${MATERIALS_SRCS}")

target_link_libraries(materials PUBLIC tst::engine)
