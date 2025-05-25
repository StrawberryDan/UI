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
	FontMap::FontMap(FontFace& fontFace, FT_Render_Mode renderMode, Core::Math::Vec2u pageSize)
		: mPageSize(pageSize)
		, mMaxGlyphSize(renderMode == FT_RENDER_MODE_SDF ? fontFace.GetSDFBoundingBox() : fontFace.GetBoundingBox())
		, mGlyphsPerPage(pageSize / MaxGlyphSize())
	{
		// Assert that the page size is a power of 2 for both dimensions.
		Core::Logging::WarningIf(!std::has_single_bit(pageSize[0]), "FontFace page width not set to a power of 2!");
		Core::Logging::WarningIf(!std::has_single_bit(pageSize[1]), "FontFace page height not set to a power of 2!");
		// Get the number of glyphs per page
		const auto totalGlyphsPerPage = mGlyphsPerPage.Fold(std::multiplies());

		// All of the glpyhs in this font face rendered, and split into pages.
		auto pagedGlyphs = std::views::enumerate(fontFace.RenderAllGlyphs(renderMode) | std::views::chunk(totalGlyphsPerPage));


		for (const auto& [pageIndex, pageGlyphs] : pagedGlyphs)
		{
			Page page;
			page.mAtlas = Core::Image<Core::PixelGreyscale>(mPageSize);
			for (const auto& [glyphIndex, glyph] : std::views::enumerate(pageGlyphs))
			{
				const auto offset = mGlyphsPerPage.Reversed().Unflatten(glyphIndex).Reversed() * mMaxGlyphSize;
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
