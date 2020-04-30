cmake_minimum_required(VERSION 3.16)
project(offscreen)

set(SOURCE_FILES
        init.cpp
        OffscreenCamera.cpp
        QtDrawToolGL.cpp)

set(HEADER_FILES
        OffscreenCamera.h
        QtDrawToolGL.h)

find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(SofaBase REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(offscreen Qt5::Gui GL SofaBaseVisual)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)