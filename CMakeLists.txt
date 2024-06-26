cmake_minimum_required(VERSION 3.25)
project(StrawberryUI)
include(FetchContent)


if (PROJECT_IS_TOP_LEVEL)
    FetchContent_Declare(StrawberryConfig
            GIT_REPOSITORY "https://github.com/StrawberryDan/Config.git"
            GIT_TAG "main")
    FetchContent_MakeAvailable(StrawberryConfig)
    include(StrawberryConfig)
endif()


if (NOT TARGET StrawberryUI)
    option(FT_DISABLE_BROTLI "" ON)
    option(FT_DISABLE_BZIP2 "" ON)
    option(FT_DISABLE_HARFBUZZ "" ON)
    option(FT_DISABLE_PNG "" ON)
    option(FT_DISABLE_ZLIB "" ON)
    FetchContent_Declare(FREETYPE
            GIT_REPOSITORY "https://gitlab.freedesktop.org/freetype/freetype.git"
    )
    FetchContent_MakeAvailable(FREETYPE)


    if (NOT TARGET StrawberryCore AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Core)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Core ${CMAKE_CURRENT_BINARY_DIR}/Core)
    elseif(NOT TARGET StrawberryCore)
        message(FATAL_ERROR "Failed to find Strawberry Core at ${CMAKE_CURRENT_SOURCE_DIR}/../Core")
    endif()

    if (NOT TARGET StrawberryVulkan AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Vulkan)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Vulkan ${CMAKE_CURRENT_BINARY_DIR}/Vulkan)
    elseif(NOT TARGET StrawberryGraphics)
        message(FATAL_ERROR "Failed to find Strawberry Vulkan at ${CMAKE_CURRENT_SOURCE_DIR}/../Vulkan")
    endif()

    if (NOT TARGET StrawberryAudio AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Audio)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Audio ${CMAKE_CURRENT_BINARY_DIR}/Audio)
    elseif(NOT TARGET StrawberryAudio)
        message(FATAL_ERROR "Failed to find Strawberry Audio at ${CMAKE_CURRENT_SOURCE_DIR}/../Audio")
    endif()

    add_library(StrawberryUI STATIC
			src/Strawberry/UI/Animation/Animation.cpp
			src/Strawberry/UI/Animation/Animation.hpp
            src/Strawberry/UI/Events/CallbackListener.cpp
            src/Strawberry/UI/Events/CallbackListener.hpp
            src/Strawberry/UI/Events/Dispatcher.cpp
            src/Strawberry/UI/Events/Dispatcher.hpp
            src/Strawberry/UI/Events/Event.hpp
            src/Strawberry/UI/Events/Event.hpp
            src/Strawberry/UI/Events/Listener.cpp
            src/Strawberry/UI/Events/Listener.hpp
            src/Strawberry/UI/Events/MouseButtonListener.cpp
            src/Strawberry/UI/Events/MouseButtonListener.hpp
            src/Strawberry/UI/Events/Transformer.cpp
            src/Strawberry/UI/Events/Transformer.hpp
            src/Strawberry/UI/Frame.cpp
            src/Strawberry/UI/Frame.hpp
            src/Strawberry/UI/Nodes/Node.cpp
            src/Strawberry/UI/Nodes/Node.hpp
            src/Strawberry/UI/Nodes/RectangularNode.cpp
            src/Strawberry/UI/Nodes/RectangularNode.hpp
            src/Strawberry/Rendering/Text/FontFace.cpp
            src/Strawberry/Rendering/Text/FontFace.hpp
            src/Strawberry/Rendering/Text/Freetype.cpp
            src/Strawberry/Rendering/Text/Freetype.hpp
    )
    target_link_libraries(StrawberryUI PUBLIC StrawberryCore StrawberryVulkan StrawberryAudio freetype)
    target_include_directories(StrawberryUI PUBLIC src)
    set_target_properties(StrawberryUI PROPERTIES CXX_STANDARD 20)

    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        target_link_options(StrawberryUI PRIVATE "-static")
    endif()


    add_executable(StrawberryUI_Test
        test/Main.cpp)
    target_link_libraries(StrawberryUI_Test PUBLIC StrawberryUI)
    set_target_properties(StrawberryUI_Test PROPERTIES CXX_STANDARD 20)

    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        target_link_options(StrawberryUI_Test PRIVATE "-static")
    endif()
endif()
