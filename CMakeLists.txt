cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME numerology)
set (CMAKE_CXX_STANDARD 11)

project(numerology)

option(DEBUG_MODE "Enable Debug Mode" OFF)

add_library(${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/numerology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(DEBUG_MODE)
    message("Debug Mode: ON!")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
    message("Debug Mode: OFF!")
endif(DEBUG_MODE)

set(UT ${PROJECT_NAME}_ut)
add_executable(${UT}
    ${CMAKE_CURRENT_SOURCE_DIR}/ut.cpp
)

target_link_libraries(${UT}
    ${PROJECT_NAME}
)
