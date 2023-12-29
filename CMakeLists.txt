cmake_minimum_required(VERSION 3.14)

project(menu_builder)

if(NOT DEFINED MENU_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(MENU_MASTER_PROJECT ON)
        message(STATUS "Build part: master-project")
    else()
        set(MENU_MASTER_PROJECT OFF)
        message(STATUS "Build part: sub-porject")
    endif()
endif()

option(BUILD_MENU_SIMULATOR "Build as menu SDL simulator" ${MENU_MASTER_PROJECT})

if(BUILD_MENU_SIMULATOR)
    message(STATUS "Build as menu SDL simulator")
    include(cmake/sdl_simulator.cmake)
else()
    message(STATUS "Build as menu library")
    include(cmake/libmenu.cmake)
endif()
