//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/UI/Rendering/Text/FontFace.hpp"
#include "Strawberry/UI/Rendering/Text/GlyphBitmap.hpp"
// Strawberry Core
#include "Strawberry/Core/UTF.hpp"
// Standard Library
#include <ranges>



namespace Strawberry::UI
{
	Core::Result<FontFace, Core::IO::Error> FontFace::FromFile(const std::filesystem::path& path)
	{	ZoneScoped;

		if (!std::filesystem::exists(path)) return Core::IO::Error::NotFound;

		FontFace face(nullptr);
		auto     error = FT_New_Face(FreeType::GetLibrary(), path.string().c_str(), 0, &face.mFace);
		switch (error)
		{
			case FT_Err_Ok:
				break;
			default:
				Core::Unreachable();
		}

		FT_Set_Pixel_Sizes(face.mFace, face.mFace->units_per_EM >> 6, 0);
		return face;
	}


	FontFace::FontFace(FontFace&& other) noexcept
		: mFace(std::exchange(other.mFace, nullptr)) {}


	FontFace& FontFace::operator=(FontFace&& other) noexcept
	{
		if (this != &other)
		{
			std::destroy_at(this);
			std::construct_at(this, std::move(other));
		}
		return *this;
	}


	FontFace::~FontFace()
	{
		if (mFace)
		{
			FT_Done_Face(mFace);
		}
	}


	Glyph FontFace::LoadGlyphAtIndex(Glyph::Index glyphIndex)
	{	ZoneScoped;
		LoadGlyph(glyphIndex);


		FT_Outline&        outline = mFace->glyph->outline;
		Glyph::ContourList contours(outline.n_contours);
		int                contourIndex = 0;
		for (int i = 0; i < outline.n_points; i++)
		{
			// Copy contour data to the glyph
			contours[contourIndex].emplace_back(
				Glyph::Point
				{
					.position = {outline.points[i].x, outline.points[i].y}
				});

			// If we've reached the end of this contour, increment the contourIndex
			if (i == outline.contours[contourIndex])
			{
				++contourIndex;
			}
		}

		for (auto& contour : contours)
		{
			contour.shrink_to_fit();
		}

		return Glyph(glyphIndex, contours);
	}


	Core::Optional<Glyph> FontFace::LoadGlyphOfCodepoint(char32_t codepoint)
	{	ZoneScoped;
		if (auto index = GetIndexOfChar(codepoint))
		{
			return LoadGlyphAtIndex(*index);
		}

		return Core::NullOpt;
	}


	Core::Math::Vec2u FontFace::GetBoundingBox() const
	{
		auto boundingBox = Core::Math::Vec2u((mFace->bbox.xMax - mFace->bbox.xMin >> 6) + 2, (mFace->bbox.yMax - mFace->bbox.yMin >> 6) + 2);
		boundingBox[0] = (boundingBox[0] * mFace->size->metrics.x_ppem) / 16;
		boundingBox[1] = (boundingBox[1] * mFace->size->metrics.y_ppem) / 16;
		return boundingBox;
	}

	Core::Math::Vec2u FontFace::GetSDFBoundingBox() const
	{
		return GetBoundingBox().Map([] (auto&& x) { return x + 16; });
	}


	Core::Optional<GlyphBitmap> FontFace::RenderGlyph(char32_t codepoint, FT_Render_Mode renderMode)
	{	ZoneScoped;
		Core::Optional<Glyph::Index> index = GetIndexOfChar(codepoint);
		if (!index)
			return Core::NullOpt;

		LoadGlyph(*index);

		if (FT_Render_Glyph(mFace->glyph, renderMode) != 0)
		{
			Core::Logging::Warning("Failed to load codepoint {} from font face!", Core::ToUTF8(codepoint).UnwrapOr("N/A"));
			return Core::NullOpt;
		}

		FT_Bitmap& bitmap = mFace->glyph->bitmap;

		Core::IO::DynamicByteBuffer bytes = Core::IO::DynamicByteBuffer::WithCapacity(bitmap.rows * bitmap.width);
		for (int y = 0; y < bitmap.rows; y++)
		{
			for (int x = 0; x < bitmap.width; x++)
			{
				bytes.Push<uint8_t>(bitmap.buffer[y * bitmap.pitch + x]);
			}
		}

		Core::Image<Core::PixelGreyscale> image(bitmap.width, bitmap.rows, std::move(bytes));

		return GlyphBitmap(codepoint, std::move(image));
	}


	std::vector<GlyphBitmap> FontFace::RenderAllGlyphs(FT_Render_Mode renderMode)
	{	ZoneScoped;
		std::vector<GlyphBitmap> result;
		result.reserve(mFace->num_glyphs);

		FT_UInt  index;
		FT_ULong charcode = FT_Get_First_Char(mFace, &index);
		while (index != 0)
		{
			if (auto bitmap = RenderGlyph(charcode, renderMode))
			{
				result.emplace_back(bitmap.Unwrap());
			}
			charcode = FT_Get_Next_Char(mFace, charcode, &index);
		}

		return result;
	}


	Core::Optional<FT_UInt> FontFace::GetIndexOfChar(char32_t codepoint) const
	{	ZoneScoped;
		FT_UInt ret = FT_Get_Char_Index(mFace, codepoint);
		if (ret == 0)
		[[unlikely]]
		{
			return Core::NullOpt;
		}
		return ret;
	}

	void FontFace::LoadGlyph(Glyph::Index glyphIndex)
	{
		// Check whether this glyph is already loaded.
		if (mFace->glyph->glyph_index != glyphIndex)
		{
			auto error = FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_DEFAULT);
			Core::AssertEQ(error, 0);
		}
	}


	FontFace::FontFace(FT_Face face)
		: mFace(face) {}
}
