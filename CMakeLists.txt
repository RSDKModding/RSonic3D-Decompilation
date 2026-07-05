cmake_minimum_required(VERSION 3.20)
project(RetroEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RETRO_FORCE_CASE_INSENSITIVE "Forces case insensivity." OFF)
option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)
option(RETRO_ORIGINAL_CODE "Removes any change that differs from the original code." OFF)

set(RETRO_SDL_VERSION 3 CACHE STRING "Select between SDL3, SDL2 and SDL1, defaults to SDL3")
set(RETRO_OUTPUT_NAME "RSonic3D" CACHE STRING "The exported name of the executable.")

if(NOT WIN32 OR NOT RETRO_ORIGINAL_CODE)
    find_package(GLEW CONFIG)

    if(NOT GLEW_FOUND)
        message(NOTICE "could not find glew, attempting to build from source")
    else()
        message("found GLEW")
        add_library(glew ALIAS GLEW::GLEW)
    endif()

    if(RETRO_SDL_VERSION STREQUAL "3")
        find_package(SDL3 CONFIG REQUIRED)
    elseif(RETRO_SDL_VERSION STREQUAL "2")
        find_package(SDL2 CONFIG REQUIRED)
    elseif(RETRO_SDL_VERSION STREQUAL "1")
        find_package(SDL1 CONFIG REQUIRED)
    else()
        message(FATAL_ERROR "RETRO_SDL_VERSION must be 1, 2 or 3")
    endif()
endif()

if(WIN32 AND RETRO_ORIGINAL_CODE)
    find_package(freeimage CONFIG QUIET)
    if(freeimage_FOUND)
        message(STATUS "Found FreeImage")
    else()
        find_path(FREEIMAGE_INCLUDE_DIR NAMES FreeImage.h)
        find_library(FREEIMAGE_LIBRARY NAMES freeimage FreeImage)
        if(NOT FREEIMAGE_INCLUDE_DIR OR NOT FREEIMAGE_LIBRARY)
            message(FATAL_ERROR "FreeImage not found")
        endif()
    endif()
endif()

set(RETRO_FILES
    dependencies/coldet/src/box.cpp
    dependencies/coldet/src/box_bld.cpp
    dependencies/coldet/src/cdmath3d.cpp
    dependencies/coldet/src/coldet.cpp
    dependencies/coldet/src/coldet_bld.cpp
    dependencies/coldet/src/multiobject.cpp
    dependencies/coldet/src/mytritri.cpp
    dependencies/coldet/src/sysdep.cpp
    dependencies/coldet/src/tritri.c
    RetroSonic/DX7/ddutil.cpp
    RetroSonic/DX7/texture.cpp
    RetroSonic/Collision.cpp
    RetroSonic/Drawing.cpp
    RetroSonic/fcaseopen.c
    RetroSonic/Ini.cpp
    RetroSonic/Input.cpp
    RetroSonic/Level.cpp
    RetroSonic/main.cpp
    RetroSonic/Math.cpp
    RetroSonic/ModAPI.cpp
    RetroSonic/Object.cpp
    RetroSonic/Player.cpp
    RetroSonic/Reader.cpp
    RetroSonic/RetroEngine.cpp
    RetroSonic/String.cpp
    RetroSonic/Text.cpp
    RetroSonic/TitleScreen.cpp
    RetroSonic/Userdata.cpp
)

if(NOT PLATFORM)
    if(WIN32)
        set(PLATFORM "Windows" CACHE STRING "The platform to compile for.")
    elseif(ANDROID)
        set(PLATFORM "Android" CACHE STRING "The platform to compile for.")
    else()
        set(PLATFORM ${CMAKE_SYSTEM_NAME} CACHE STRING "The platform to compile for.")
    endif()
endif()

add_executable(RetroEngine WIN32 ${RETRO_FILES} RetroSonic/RetroSonic.rc)

target_compile_definitions(RetroEngine PRIVATE _CRT_SECURE_NO_WARNINGS D3D_OVERLOADS SDL_MAIN_HANDLED)
set_target_properties(RetroEngine PROPERTIES OUTPUT_NAME ${RETRO_OUTPUT_NAME})

if (MSVC)
    target_compile_definitions(RetroEngine PRIVATE _LIB)
else()
    target_compile_definitions(RetroEngine PRIVATE GCC)
endif()

target_include_directories(RetroEngine PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/coldet/src
    ${CMAKE_SOURCE_DIR}/RetroSonic
    ${FREEIMAGE_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
)

if(WIN32 AND RETRO_ORIGINAL_CODE)
    ## i dunno the correct way to check for the DX7SDK's install directory
    target_include_directories(RetroEngine PRIVATE C:\\DX7SDK\\include)
    target_link_directories(RetroEngine PRIVATE C:\\DX7SDK\\lib)
    target_link_libraries(RetroEngine PRIVATE dinput dxguid ddraw winmm)

    if(freeimage_FOUND)
        target_link_libraries(RetroEngine PRIVATE freeimage::FreeImage)
    else()
        target_link_libraries(RetroEngine PRIVATE ${FREEIMAGE_LIBRARY})
    endif()

    set_target_properties(RetroEngine PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_link_libraries(RetroEngine PRIVATE glew)

    if(RETRO_SDL_VERSION STREQUAL "3")
        target_link_libraries(RetroEngine PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL3::SDL3main>
            $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
        )
    elseif(RETRO_SDL_VERSION STREQUAL "2")
        target_link_libraries(RetroEngine PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        )
    elseif(RETRO_SDL_VERSION EQUAL 1)
        target_link_libraries(RetroEngine PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL1::SDL1main>
            $<IF:$<TARGET_EXISTS:SDL1::SDL1>,SDL1::SDL1,SDL1::SDL1-static>
        )

        if(WIN32)
            ## Why?
            target_link_libraries(RetroEngine PRIVATE winmm dxguid)
        endif()
    endif()
endif()

target_compile_definitions(RetroEngine PRIVATE
    RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
    RETRO_USE_ORIGINAL_CODE=$<BOOL:${RETRO_ORIGINAL_CODE}>
    FORCE_CASE_INSENSITIVE=$<BOOL:${RETRO_FORCE_CASE_INSENSITIVE}>
    RETRO_USE_SDL${RETRO_SDL_VERSION}=1
)