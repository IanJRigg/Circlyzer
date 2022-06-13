cmake_minimum_required(VERSION 3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Application Configuration
###############################################################################
set(APPLICATION_NAME ${PROJECT_NAME})
set(PROEJCT_SO_VERSION 1)

set(CIRCUIT_ANALYZER_INCLUDE_DIR ${INCLUDE_DIR}/circuit_analyzer/)

set(SOURCE_FILES
    network.cpp
    phasors.cpp
)

set(PUBLIC_HEADER_FILES
    ${CIRCUIT_ANALYZER_INCLUDE_DIR}/component.h
    ${CIRCUIT_ANALYZER_INCLUDE_DIR}/network.h
    ${CIRCUIT_ANALYZER_INCLUDE_DIR}/phasors.h
    ${CIRCUIT_ANALYZER_INCLUDE_DIR}/units.h
)

set(PRIVATE_HEADER_FILES

)

add_library(
    ${APPLICATION_NAME}
    ${SOURCE_FILES}
    ${PUBLIC_HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
)

set_target_properties(
    ${APPLICATION_NAME}
    PROPERTIES
        # Project specific versions
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROEJCT_SO_VERSION}

        # Public Header file location
        PUBLIC_HEADER "${HEADER_FILES}"
)

target_compile_options(
    ${APPLICATION_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

target_include_directories(
    ${APPLICATION_NAME}
    PRIVATE
        ${SRC_DIR}
        ${INCLUDE_DIR}
)

target_link_libraries(
    ${APPLICATION_NAME}
    PUBLIC
)