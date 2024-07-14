#include "FontFace.hpp"

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
				return face;
			default:
				Core::Unreachable();
		}
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


		for (auto& contour: contours)
		{
			contour.shrink_to_fit();
		}


		return Glyph(contours);
	}


	Core::Image<Core::PixelGreyscale> FontFace::RenderGlyph(unsigned int glyphIndex)
	{
		if (mFace->glyph->glyph_index != glyphIndex)
			Core::AssertEQ(FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_DEFAULT), 0);

		Core::AssertEQ(FT_Render_Glyph(mFace->glyph, FT_RENDER_MODE_NORMAL), 0);

		FT_Bitmap& bitmap = mFace->glyph->bitmap;
		return {bitmap.width, bitmap.rows, Core::IO::DynamicByteBuffer(bitmap.buffer, bitmap.width * bitmap.rows)};
	}


	Core::Image<Core::PixelF32Greyscale> FontFace::RenderSDF(unsigned int glyphIndex, float spread)
	{
		if (mFace->glyph->glyph_index != glyphIndex)
			Core::AssertEQ(FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_DEFAULT), 0);

		Core::AssertEQ(FT_Render_Glyph(mFace->glyph, FT_RENDER_MODE_SDF), 0);

		auto normaliseSD = [spread](uint8_t x)
		{
			return spread * (static_cast<float>(x) - 128.0f) / 128.0f;
		};

		FT_Bitmap&           bitmap = mFace->glyph->bitmap;
		std::vector<uint8_t> values(bitmap.buffer, bitmap.buffer + bitmap.width * bitmap.rows);
		auto                 converted = values | std::views::transform(normaliseSD) | std::ranges::to<std::vector>();

		return {bitmap.width, bitmap.rows, Core::IO::DynamicByteBuffer(converted)};
	}


	FontFace::FontFace(FT_Face face)
		: mFace(face) {}
}
