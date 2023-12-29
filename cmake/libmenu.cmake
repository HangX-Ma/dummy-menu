cmake_minimum_required(VERSION 3.14)

project(menu LANGUAGES CXX C)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build shared option
option(MENU_BUILD_SHARED "Build shared library" OFF)

# build position independent code
option(MENU_BUILD_PIC "Build position independent code (-fPIC)" OFF)

if(MENU_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

file(GLOB MENU_ALL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/menu/*.h)
file(GLOB MENU_ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if(MENU_BUILD_SHARED OR BUILD_SHARED_LIBS)
    add_library(menu SHARED ${MENU_ALL_SOURCES} ${MENU_ALL_HEADERS})
else()
    add_library(menu STATIC ${MENU_ALL_SOURCES} ${MENU_ALL_HEADERS})
endif()

add_library(menu::menu ALIAS menu)
target_include_directories(
    menu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)