cmake_minimum_required(VERSION 3.25)
project(StrawberryUI)
include(FetchContent)

add_compile_options("-fdiagnostics-all-candidates")

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
		src/Strawberry/UI/Rendering/RenderPipeline.hpp
		src/Strawberry/UI/Rendering/RenderPipeline.cpp
		src/Strawberry/UI/Rendering/Text/FontFace.cpp
		src/Strawberry/UI/Rendering/Text/FontFace.hpp
		src/Strawberry/UI/Rendering/Text/FontMap.cpp
		src/Strawberry/UI/Rendering/Text/FontMap.hpp
		src/Strawberry/UI/Rendering/Text/Freetype.cpp
		src/Strawberry/UI/Rendering/Text/Freetype.hpp
		src/Strawberry/UI/Rendering/Text/Glyph.cpp
		src/Strawberry/UI/Rendering/Text/Glyph.hpp
		src/Strawberry/UI/Rendering/Text/GlyphBitmap.hpp
	)
	target_link_libraries(StrawberryUI PUBLIC StrawberryCore StrawberryVulkan StrawberryAudio freetype)
	target_include_directories(StrawberryUI PUBLIC src)


	new_strawberry_executable(NAME StrawberryUI_Test SOURCE
		test/Main.cpp)
	target_link_libraries(StrawberryUI_Test PUBLIC StrawberryUI)
endif ()
