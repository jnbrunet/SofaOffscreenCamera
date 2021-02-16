cmake_minimum_required(VERSION 3.12)
project(SofaOffscreenCameraBindings VERSION 20.12 LANGUAGES CXX)

set(HEADER_FILES
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaOffscreenCamera/Bindings/Binding_OffscreenCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaOffscreenCamera/Bindings/ModuleSofaOffscreenCamera.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED)
endif()

SP3_add_python_module (
    TARGET       ${PROJECT_NAME}
    PACKAGE      SofaOffscreenCamera
    MODULE       SofaOffscreenCamera
#    DESTINATION  OffscreenCamera
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaOffscreenCamera SofaPython3::Bindings
)