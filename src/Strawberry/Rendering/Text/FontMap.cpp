//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "FontMap.hpp"

#include <ranges>

#include "Strawberry/Core/UTF.hpp"

//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	FontMap::FontMap(FontFace& fontFace, Core::Math::Vec2u pageSize)
		: mPageSize(pageSize)
		, mMaxGlyphSize(fontFace.GetBoundingBox())
		, mGlyphsPerPage(pageSize / GetBoundingBoxSize())
	{
		auto totalGlyphsPerPage = mGlyphsPerPage[0] * mGlyphsPerPage[1];

		auto allGlyphs = fontFace.RenderAllGlyphs();
		unsigned pageIndex = 0;
		for (const auto& chunk : allGlyphs | std::views::chunk(totalGlyphsPerPage))
		{
			Page page;
			page.mAtlas = Core::Image<Core::PixelGreyscale>(mPageSize);
			int i = 0;
			for (auto glyph : chunk)
			{
				auto offset = Core::Math::Vec2u(i % mGlyphsPerPage[0], i / mGlyphsPerPage[0]) * mMaxGlyphSize;
				page.mAtlas.Blit(glyph.Bitmap(), offset);
				i++;
			}
			page.mAtlas.Save(fmt::format("Thing{}.png", pageIndex++));
		}
	}
}
