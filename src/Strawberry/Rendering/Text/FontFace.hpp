#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include "Glyph.hpp"
#include <Strawberry/Core/Types/Result.hpp>
#include <Strawberry/Core/IO/Error.hpp>
#include "Freetype.hpp"
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


		Glyph  LoadGlyph(unsigned int glyphIndex);
		Bitmap LoadBitmap(unsigned int glyphIndex);

	protected:
		FontFace(FT_Face face);

	private:
		FT_Face mFace;
	};
}
