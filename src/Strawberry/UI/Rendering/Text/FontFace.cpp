//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/UI/Rendering/Text/FontFace.hpp"
#include "Strawberry/UI/Rendering/Text/GlyphBitmap.hpp"
// Standard Library
#include <ranges>


namespace Strawberry::UI
{
	Core::Result<FontFace, Core::IO::Error> FontFace::FromFile(const std::filesystem::path& path)
	{
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

		FT_Set_Pixel_Sizes(face.mFace, 1 * face.mFace->units_per_EM >> 6, 0);
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


	Glyph FontFace::LoadGlyph(unsigned int glyphIndex)
	{
		auto error = FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_DEFAULT);
		Core::AssertEQ(error, 0);

		FT_Outline&        outline = mFace->glyph->outline;
		Glyph::ContourList contours(outline.n_contours);
		int                contour = 0;
		for (int i = 0; i < outline.n_points; i++)
		{
			contours[contour].emplace_back(Glyph::Point{.position = {outline.points[i].x, outline.points[i].y}});

			if (i == outline.contours[contour])
			{
				++contour;
			}
		}


		for (auto& contour : contours)
		{
			contour.shrink_to_fit();
		}


		return Glyph(glyphIndex, contours);
	}


	Core::Math::Vec2u FontFace::GetBoundingBox() const
	{
		return {(mFace->bbox.xMax - mFace->bbox.xMin) >> 6, (mFace->bbox.yMax - mFace->bbox.yMin) >> 6};
	}


	Core::Optional<GlyphBitmap> FontFace::RenderGlyph(uint32_t codepoint)
	{
		Core::Optional<Glyph::Index> index = GetIndexOfChar(codepoint);
		if (!index)
			return Core::NullOpt;

		if (mFace->glyph->glyph_index != codepoint)
			Core::AssertEQ(FT_Load_Glyph(mFace, index.Value(), FT_LOAD_DEFAULT), 0);

		Core::AssertEQ(FT_Render_Glyph(mFace->glyph, FT_RENDER_MODE_NORMAL), 0);

		FT_Bitmap& bitmap = mFace->glyph->bitmap;

		Core::IO::DynamicByteBuffer bytes;
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


	std::vector<GlyphBitmap> FontFace::RenderAllGlyphs()
	{
		std::vector<GlyphBitmap> result;
		result.reserve(mFace->num_glyphs);

		FT_UInt  index;
		FT_ULong charcode = FT_Get_First_Char(mFace, &index);
		while (index != 0)
		{
			if (auto bitmap = RenderGlyph(charcode))
			{
				result.emplace_back(bitmap.Unwrap());
			}
			charcode = FT_Get_Next_Char(mFace, charcode, &index);
		}

		return result;
	}


	Core::Optional<FT_UInt> FontFace::GetIndexOfChar(char32_t codepoint) const
	{
		FT_UInt ret = FT_Get_Char_Index(mFace, codepoint);
		if (ret == 0)
		[[unlikely]]
		{
			return Core::NullOpt;
		}
		return ret;
	}


	FontFace::FontFace(FT_Face face)
		: mFace(face) {}
}
