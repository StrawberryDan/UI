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
		, mGlyphsPerPage(pageSize / MaxGlyphSize())
	{
		Core::Assert(std::has_single_bit(pageSize[0]));
		Core::Assert(std::has_single_bit(pageSize[1]));

		auto totalGlyphsPerPage = mGlyphsPerPage[0] * mGlyphsPerPage[1];

		auto pagedGlyphs = std::views::enumerate(fontFace.RenderAllGlyphs() | std::views::chunk(totalGlyphsPerPage));
		for (const auto& [pageIndex, pageGlyphs] : pagedGlyphs)
		{
			Page page;
			page.mAtlas = Core::Image<Core::PixelGreyscale>(mPageSize);
			for (const auto& [glyphIndex, glyph] : std::views::enumerate(pageGlyphs))
			{
				const auto offset = Core::Math::Vec2u(glyphIndex % mGlyphsPerPage[0], glyphIndex / mGlyphsPerPage[0]) * mMaxGlyphSize;
				page.mAtlas.Blit(glyph.bitmap, offset);
				mGlyphs.emplace(glyph.codepoint,
				                GlyphAddress{
					                .pageIndex = static_cast<unsigned>(pageIndex),
					                .offset = offset,
					                .extent = glyph.bitmap.Size()
				                });
			}
		}
	}


	Core::Optional<FontMap::GlyphAddress> FontMap::GetGlyphAddress(uint32_t codepoint) const noexcept
	{
		auto search = mGlyphs.find(codepoint);
		return search == mGlyphs.end() ? Core::NullOpt : Core::Optional(search->second);
	}


	Core::Optional<const FontMap::Page*> FontMap::GetPage(PageIndex pageIndex) const noexcept
	{
		if (pageIndex < mPages.size())
		[[likely]]
		{
			return &mPages[pageIndex];
		}

		return Core::NullOpt;
	}


	FontMap::PageIndex FontMap::GetPageCount() const noexcept
	{
		return mPages.size();
	}
}
