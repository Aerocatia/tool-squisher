cmake_minimum_required(VERSION 4.0)

project(ToolSquisher
    VERSION 1.3.0
    DESCRIPTION "Tool to fix Halo CE map errors caused by tool.exe bugs"
    LANGUAGES C
)

add_executable(tool-squisher
    src/tool-squisher.c
)

if(WIN32)
    target_sources(tool-squisher PRIVATE src/windows.rc)
endif()
