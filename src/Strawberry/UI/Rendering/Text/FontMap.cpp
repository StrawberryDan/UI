//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "FontMap.hpp"
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
	{
		// Assert that the page size is a power of 2 for both dimensions.
		Core::Logging::WarningIf(!std::has_single_bit(pageSize[0]), "FontFace page width not set to a power of 2!");
		Core::Logging::WarningIf(!std::has_single_bit(pageSize[1]), "FontFace page height not set to a power of 2!");

		Core::Math::Vec2u offset;
		unsigned rowMaxHeight = 0;

		auto glyphs = fontFace.RenderAllGlyphs(renderMode);
		std::ranges::sort(glyphs, std::less(), [] (auto&& x) { return x.bitmap.Height(); });


		mPages.emplace_back();
		mPages.back().mAtlas = Core::Image<Core::PixelGreyscale>(mPageSize);
		for (auto&& glyph : glyphs)
		{
			Core::Assert(glyph.bitmap.Width() < mPageSize[0]);
			Core::Assert(glyph.bitmap.Height() < mPageSize[1]);
			if (offset[0] > mPageSize[0] - glyph.bitmap.Width())
			{
				offset[0] = 0;
				offset[1] += rowMaxHeight;
				rowMaxHeight = 0;
			}

			if (offset[1] > mPageSize[1] - glyph.bitmap.Height())
			{
				mPages.emplace_back();
				mPages.back().mAtlas = Core::Image<Core::PixelGreyscale>(mPageSize);
				offset = Core::Math::Vec2u(0, 0);
				rowMaxHeight = 0;
			}

			mPages.back().mAtlas.Blit(glyph.bitmap, offset);
			offset[0] += glyph.bitmap.Width();
			rowMaxHeight = std::max(rowMaxHeight, glyph.bitmap.Height());

			mGlyphs.emplace(
				glyph.codepoint,
				GlyphAddress
				{
					.pageIndex = static_cast<unsigned>(mPages.size() - 1),
					.offset = offset,
					.extent = glyph.bitmap.Size()
				});
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
