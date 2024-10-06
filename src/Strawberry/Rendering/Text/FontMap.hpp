#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/Rendering/Text/FontFace.hpp"
// Standard Library
#include <map>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class FontMap
	{
	public:
		using PageIndex = unsigned;


		struct Page
		{
			Core::Image<Core::PixelGreyscale> mAtlas;
		};


		struct GlyphAddress
		{
			// Index of the page this glyph is on.
			unsigned pageIndex = 0;
			// In multiples of mMaxGlyphSize.
			Core::Math::Vec2u offset;
			// In pixels.
			Core::Math::Vec2u extent;
		};


		FontMap(FontFace& fontFace, Core::Math::Vec2u pageSize);


		[[nodiscard]] Core::Math::Vec2u MaxGlyphSize() const noexcept
		{
			return mMaxGlyphSize;
		}


		[[nodiscard]] Core::Optional<FontMap::GlyphAddress> GetGlyphAddress(uint32_t codepoint) const noexcept;
		[[nodiscard]] Core::Optional<const Page*>           GetPage(PageIndex pageIndex) const noexcept;
		[[nodiscard]] PageIndex                             GetPageCount() const noexcept;

	private:
		Core::Math::Vec2u                mPageSize;
		Core::Math::Vec2u                mMaxGlyphSize;
		Core::Math::Vec2u                mGlyphsPerPage;
		std::vector<Page>                mPages;
		std::map<char32_t, GlyphAddress> mGlyphs;
	};
}
