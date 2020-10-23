# This file is part of Tempest project
# Author: Karol Kontny

function(add_imgui)

    add_library(imgui)
    
    add_library(tst::imgui ALIAS imgui)
    
    set(IMGUI_HDRS
        ${PROJECT_SOURCE_DIR}/libs/imgui/imconfig.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_internal.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imconfig.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imstb_rectpack.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imstb_textedit.h
        ${PROJECT_SOURCE_DIR}/libs/imgui/imstb_truetype.h
    )

    set(IMGUI_SRCS 
        ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    )
    
    set(all_srcs ${IMGUI_HDRS} ${IMGUI_SRCS})

    source_group(imgui FILES ${all_srcs})
    
    target_sources(imgui PUBLIC ${IMGUI_HDRS} PRIVATE ${IMGUI_SRCS})
    
    target_include_directories(imgui PUBLIC "${PROJECT_SOURCE_DIR}/libs/")

    target_compile_options(imgui PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
    target_compile_options(imgui PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")
    target_compile_options(imgui PUBLIC "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEBINFO_COMPILE_OPTIONS}>")

    target_compile_definitions(imgui PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_DEFINITIONS}>")
    target_compile_definitions(imgui PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_DEFINITIONS}>")
    target_compile_definitions(imgui PUBLIC "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEBINFO_COMPILE_DEFINITIONS}>")

    set_target_properties(imgui PROPERTIES LINK_FLAGS_DEBUG "${DEBUG_LINK_OPTIONS}" LINK_FLAGS_RELEASE "${RELEASE_LINK_OPTIONS}" LINKER_LANGUAGE CXX)

endfunction()