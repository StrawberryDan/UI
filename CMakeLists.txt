cmake_minimum_required(VERSION 3.25)
project(StrawberryUI)
include(FetchContent)


if (PROJECT_IS_TOP_LEVEL)
	FetchContent_Declare(StrawberryConfig
		GIT_REPOSITORY "https://github.com/StrawberryDan/Config.git"
		GIT_TAG "main")
	FetchContent_MakeAvailable(StrawberryConfig)
	include(StrawberryConfig)
endif ()


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


	find_strawberry_library(NAMES Core Vulkan Audio)


	new_strawberry_library(NAME StrawberryUI SOURCE
		src/Strawberry/Rendering/Text/FontFace.cpp
		src/Strawberry/Rendering/Text/FontFace.hpp
		src/Strawberry/Rendering/Text/FontMap.cpp
		src/Strawberry/Rendering/Text/FontMap.hpp
		src/Strawberry/Rendering/Text/Freetype.cpp
		src/Strawberry/Rendering/Text/Freetype.hpp
		src/Strawberry/Rendering/Text/Glyph.cpp
		src/Strawberry/Rendering/Text/Glyph.hpp
		src/Strawberry/Rendering/Text/GlyphBitmap.hpp
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
	)
	target_link_libraries(StrawberryUI PUBLIC StrawberryCore StrawberryVulkan StrawberryAudio freetype)
	target_include_directories(StrawberryUI PUBLIC src)


	new_strawberry_executable(NAME StrawberryUI_Test SOURCE
		test/Main.cpp)
	target_link_libraries(StrawberryUI_Test PUBLIC StrawberryUI)
endif ()
