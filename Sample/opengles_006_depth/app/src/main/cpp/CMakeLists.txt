CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
SET(NAME_PROJECT "opengles_006_depth")
PROJECT(${NAME_PROJECT})
if (DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()
    MESSAGE(STATUS "Set Debug Build !")
endif ()

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")

#ASSET_FILES
SET(ASSET_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Bin/Assets")
SET(ASSET_DST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../assets")
FILE(COPY ${ASSET_SRC_DIR}/Mesh       DESTINATION ${ASSET_DST_DIR})
FILE(COPY ${ASSET_SRC_DIR}/Shader     DESTINATION ${ASSET_DST_DIR})
FILE(COPY ${ASSET_SRC_DIR}/Texture    DESTINATION ${ASSET_DST_DIR})


#HEADER_FILES
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES_CPP
FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")


#LIB_INCLUDE
SET(LIB_INCLUDE ${ANDROID_NDK}/sysroot/usr/include
                ${ANDROID_NDK}/sources/android/native_app_glue
                ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/assimp-5.2.4"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/glm-0.9.9.8"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/glad-3.2"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/imgui-1.85_gles3_android"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/libnoise-1.0.0"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/spdlog-1.10.0"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Include/Android/stb-0.02"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../LostPeterOpenGLES/LostPeterFoundation/include"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../LostPeterOpenGLES/LostPeterOpenGLES/include")
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_DEPENDENCIES_DIR, LIB_PLUGINS_DIR
SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Dependencies/Lib/Android/arm64-v8a") 
SET(LIB_PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Plugins/Android/arm64-v8a")
MESSAGE(STATUS "LIB_DEPENDENCIES_DIR dir: ${LIB_DEPENDENCIES_DIR}")
MESSAGE(STATUS "LIB_PLUGINS_DIR dir: ${LIB_PLUGINS_DIR}")


if (DEBUG)
  LINK_LIBRARIES(${LIB_DEPENDENCIES_DIR}/libassimp-5.2.4_d.a
                 ${LIB_DEPENDENCIES_DIR}/libglad-3.2_d.a
                 ${LIB_DEPENDENCIES_DIR}/libimgui-1.85_gles3_android_d.a
                 ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0_d.a
                 ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0_d.a
                 ${LIB_DEPENDENCIES_DIR}/libtinyxml-2.6.2_d.a
                 ${LIB_DEPENDENCIES_DIR}/libzlib-1.2.11_d.a)
else ()
  LINK_LIBRARIES(${LIB_DEPENDENCIES_DIR}/libassimp-5.2.4.a
                 ${LIB_DEPENDENCIES_DIR}/libglad-3.2.a
                 ${LIB_DEPENDENCIES_DIR}/libimgui-1.85_gles3_android.a
                 ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0.a
                 ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0.a
                 ${LIB_DEPENDENCIES_DIR}/libtinyxml-2.6.2.a
                 ${LIB_DEPENDENCIES_DIR}/libzlib-1.2.11.a)
endif()
ADD_DEFINITIONS("--sysroot=${ANDROID_NDK}/sysroot -Wall")
ADD_LIBRARY(${PROJECT_NAME} SHARED 
            ${HEADER_FILES} ${SOURCE_FILES_CPP}
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_ES3)

if (DEBUG)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE
                        android
                        log
                        GLESv3
                        EGL
                        ${LIB_PLUGINS_DIR}/libLostPeterFoundation_d.so
                        ${LIB_PLUGINS_DIR}/libLostPeterOpenGLES_d.so)
else ()
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE
                        android
                        log
                        GLESv3
                        EGL
                        ${LIB_PLUGINS_DIR}/libLostPeterFoundation.so
                        ${LIB_PLUGINS_DIR}/libLostPeterOpenGLES.so)
endif()