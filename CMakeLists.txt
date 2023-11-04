cmake_minimum_required(VERSION 3.25)
project(StrawberryUI)


if (NOT TARGET StrawberryUI)

    if (NOT TARGET StrawberryCore AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Core)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Core ${CMAKE_CURRENT_BINARY_DIR}/Core)
    elseif(NOT TARGET StrawberryCore)
        message(FATAL_ERROR "Failed to find Strawberry Core at ${CMAKE_CURRENT_SOURCE_DIR}/../Core")
    endif()

    if (NOT TARGET StrawberryGraphics AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Graphics)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Graphics ${CMAKE_CURRENT_BINARY_DIR}/Graphics)
    elseif(NOT TARGET StrawberryGraphics)
        message(FATAL_ERROR "Failed to find Strawberry Graphics at ${CMAKE_CURRENT_SOURCE_DIR}/../Graphics")
    endif()

    if (NOT TARGET StrawberryAudio AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Audio)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Audio ${CMAKE_CURRENT_BINARY_DIR}/Audio)
    elseif(NOT TARGET StrawberryAudio)
        message(FATAL_ERROR "Failed to find Strawberry Audio at ${CMAKE_CURRENT_SOURCE_DIR}/../Audio")
    endif()


    add_library(StrawberryUI
        src/Node.hpp
        src/Node.cpp)
    target_link_libraries(StrawberryUI PUBLIC StrawberryCore StrawberryGraphics StrawberryAudio)
    set_target_properties(StrawberryUI PROPERTIES CXX_STANDARD 20)


    add_executable(StrawberryUI_Test
        test/Main.cpp)
    target_link_libraries(StrawberryUI_Test PUBLIC StrawberryUI)
    set_target_properties(StrawberryUI_Test PROPERTIES CXX_STANDARD 20)
endif()