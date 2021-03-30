cmake_minimum_required(VERSION 3.12)
project(SofaOffscreenCamera VERSION 20.12 LANGUAGES CXX)

# Detect if this is a subproject of SOFA
if ("${CMAKE_PROJECT_NAME}" STREQUAL "Sofa")
    set(COMPILED_AS_SOFA_SUBPROJECT 1)
    message(STATUS "${PROJECT_NAME} is compiled as a subproject to SOFA.")
else()
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "install")
    endif()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED) # Dependency to SofaBaseVisual
find_package(Qt5 COMPONENTS Gui REQUIRED) # Dependency to Qt5::Gui
find_package(GLEW QUIET REQUIRED)

set(SOURCE_FILES
    src/SofaOffscreenCamera/init.cpp
    src/SofaOffscreenCamera/OffscreenCamera.cpp
    src/SofaOffscreenCamera/GlewProxy.cpp
    src/SofaOffscreenCamera/QtDrawToolGL.cpp
)

set(HEADER_FILES
    src/SofaOffscreenCamera/OffscreenCamera.h
    src/SofaOffscreenCamera/GlewProxy.h
    src/SofaOffscreenCamera/QtDrawToolGL.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Gui SofaBaseVisual)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Create package Config, Version & Target files.
sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${PROJECT_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR ${PROJECT_NAME}
    EXAMPLE_INSTALL_DIR "share/examples"
    RELOCATABLE "plugins"
)

find_package(pybind11 QUIET) # This is needed to get the latest installed version of python
find_package(SofaPython3 QUIET)
if (SofaPython3_FOUND)
    add_subdirectory(bindings)
endif()