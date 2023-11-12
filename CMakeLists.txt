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

    add_library(StrawberryUI STATIC
            src/Strawberry/UI/Node.hpp
            src/Strawberry/UI/Node.cpp
            src/Strawberry/UI/Pane.hpp
            src/Strawberry/UI/Pane.cpp
            src/Strawberry/UI/Rendering/Renderer.hpp
            src/Strawberry/UI/Rendering/Renderer.cpp
			src/Strawberry/UI/Text.cpp
			src/Strawberry/UI/Text.hpp
            src/Strawberry/UI/Frame.hpp
            src/Strawberry/UI/Frame.cpp
	)

    target_link_libraries(StrawberryUI PUBLIC StrawberryCore StrawberryGraphics StrawberryAudio)
    target_include_directories(StrawberryUI PUBLIC src)
    set_target_properties(StrawberryUI PROPERTIES CXX_STANDARD 20)
    add_target_shader(StrawberryUI ${CMAKE_CURRENT_SOURCE_DIR}/src/Strawberry/UI/Rendering/Rectangle.vert ${CMAKE_CURRENT_SOURCE_DIR}/src/Strawberry/UI/Rendering/Rectangle.vert.bin)
    add_target_shader(StrawberryUI ${CMAKE_CURRENT_SOURCE_DIR}/src/Strawberry/UI/Rendering/Rectangle.frag ${CMAKE_CURRENT_SOURCE_DIR}/src/Strawberry/UI/Rendering/Rectangle.frag.bin)

    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        target_link_options(StrawberryUI PRIVATE "-static")
    endif()


    add_executable(StrawberryUI_Test
        test/Main.cpp)
    target_link_libraries(StrawberryUI_Test PUBLIC StrawberryUI)
    set_target_properties(StrawberryUI_Test PROPERTIES CXX_STANDARD 20)
endif()
