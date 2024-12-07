#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-12-07
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
SET(NAME_PROJECT "glad-3.2")
if (DEBUG)
    PROJECT(${NAME_PROJECT}_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(${NAME_PROJECT})
endif ()

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")

#HEADER_FILES
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES_CPP
FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")

#SOURCE_FILES_C
FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

#SOURCE_FILES_MM
if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
endif ()

#LIB_INCLUDE
if (ANDROID) 
    SET(CMAKE_FIND_ROOT_PATH ${ANDROID_NDK})
    SET(LIB_INCLUDE ${ANDROID_NDK}/sysroot/usr/include
                    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
                    "${CMAKE_CURRENT_SOURCE_DIR}/include")

elseif (WIN32)
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

elseif (APPLE)
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")


endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_XXX
if (ANDROID) 
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")

    ADD_DEFINITIONS("--sysroot=${ANDROID_NDK}/sysroot -Wall")
    ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES} ${SOURCE_FILES_C})
    LINK_DIRECTORIES(${ANDROID_NDK}/sysroot/usr/lib/aarch64-linux-android)

elseif (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")

    function(ASSIGN_SOURCE_GROUP)
        foreach(_source IN ITEMS ${ARGN})
            if (IS_ABSOLUTE "${_source}")
                FILE(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
            else ()
                SET(_source_rel "${_source}")
            endif ()
            GET_FILENAME_COMPONENT(_source_path "${_source_rel}" PATH)
            STRING(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
            SOURCE_GROUP("${_source_path_msvc}" FILES "${_source}")
        endforeach()
    endfunction(ASSIGN_SOURCE_GROUP)

    function(MY_ADD_LIBRARY)
        foreach(_source IN ITEMS ${ARGN})
            ASSIGN_SOURCE_GROUP(${_source})
        endforeach()
        ADD_LIBRARY(${ARGV})
    endfunction(MY_ADD_LIBRARY)

    MY_ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C})

elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
    
    ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C} ${SOURCE_FILES_MM})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CMAKE_ENABLE_EXPORTS TRUE)   
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup -flat_namespace \
        -Wl,-search_paths_first"
    )

endif ()