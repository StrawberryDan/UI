#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include "Glyph.hpp"
#include "Freetype.hpp"
#include "Strawberry/Rendering/Text/GlyphBitmap.hpp"
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


		Glyph LoadGlyph(Glyph::Index index);


		Core::Math::Vec2u GetBoundingBox() const;


		Core::Optional<GlyphBitmap> RenderGlyph(uint32_t codepoint);
		std::vector<GlyphBitmap>    RenderAllGlyphs();

	protected:
		FontFace(FT_Face face);


		Core::Optional<FT_UInt> GetIndexOfChar(char32_t codepoint) const;

	private:
		FT_Face mFace;
	};
}
