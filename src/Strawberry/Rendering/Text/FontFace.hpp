#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include "Glyph.hpp"
#include "Freetype.hpp"
// Strawberry Core
#include "Strawberry/Core/Types/Result.hpp"
#include "Strawberry/Core/Util/Image.hpp"
#include "Strawberry/Core/IO/Error.hpp"
// Standard Library
#include <filesystem>


namespace Strawberry::UI
{
	class FontFace
			: public FreeTypeUser
	{
	public:
		static Core::Result<FontFace, Core::IO::Error> FromFile(const std::filesystem::path& path);


		FontFace(const FontFace&)            = delete;
		FontFace& operator=(const FontFace&) = delete;
		FontFace(FontFace&& other) noexcept;
		FontFace& operator=(FontFace&& other) noexcept;
		~FontFace();


		Glyph LoadGlyph(unsigned int glyphIndex);


		Core::Image<Core::PixelGreyscale>    RenderGlyph(unsigned int glyphIndex);
		Core::Image<Core::PixelF32Greyscale> RenderSDF(unsigned int glyphIndex, float spread = 2.0f);

	protected:
		FontFace(FT_Face face);

	private:
		FT_Face mFace;
	};
}
