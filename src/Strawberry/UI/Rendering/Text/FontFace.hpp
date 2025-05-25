#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include "Glyph.hpp"
#include "Freetype.hpp"
#include "Strawberry/UI/Rendering/Text/GlyphBitmap.hpp"
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
		/// Tries to load a font face from a file.
		static Core::Result<FontFace, Core::IO::Error> FromFile(const std::filesystem::path& path);


		FontFace(const FontFace&)            = delete;
		FontFace& operator=(const FontFace&) = delete;
		FontFace(FontFace&& other) noexcept;
		FontFace& operator=(FontFace&& other) noexcept;
		~FontFace();


		/// Loads the glyph at the given index
		Glyph LoadGlyphAtIndex(Glyph::Index index);
		Core::Optional<Glyph> LoadGlyphOfCodepoint(char32_t codepoint);


		/// Returns the minimum bounding box that can contain every single glyph in this font face.
		Core::Math::Vec2u GetBoundingBox() const;
		/// Returns the bounding box that can contain the SDF of every single glyph in this font face.
		Core::Math::Vec2u GetSDFBoundingBox() const;


		/// Returns a bitmap for the given code point (If this font face supports this character).
		Core::Optional<GlyphBitmap> RenderGlyph(char32_t codepoint, FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL);
		/// Returns all of the glyphs in this font face.
		std::vector<GlyphBitmap>    RenderAllGlyphs(FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL);


	protected:
		FontFace(FT_Face face);


		Core::Optional<FT_UInt> GetIndexOfChar(char32_t codepoint) const;
		void LoadGlyph(Glyph::Index index);

	private:
		FT_Face mFace;
	};
}
