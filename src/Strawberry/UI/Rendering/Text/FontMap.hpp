#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/UI/Rendering/Text/FontFace.hpp"
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
		/// The indices of the pages.
		using PageIndex = unsigned;


		/// Data releating to a page in this font map.
		struct Page
		{
			Core::Image<Core::PixelGreyscale> mAtlas;
		};


		/// Location of a glyph within this font map.
		struct GlyphAddress
		{
			/// Index of the page this glyph is on.
			unsigned pageIndex = 0;
			/// In multiples of mMaxGlyphSize.
			Core::Math::Vec2u offset;
			/// In pixels.
			Core::Math::Vec2u extent;
		};


		/// Builds a font map with the given page size out of a given font.
		/// @param fontFace The font face to render to a font map
		/// @param pageSize The size in pixels of each page of the font map.
		/// The default value is the largest supported texture size supported by all Vulkan implementations.
		explicit FontMap(FontFace& fontFace, FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL, Core::Math::Vec2u pageSize = Core::Math::Vec2u(512, 512));


		/// Get the location of the glyph of the given character code.
		[[nodiscard]] Core::Optional<GlyphAddress> GetGlyphAddress(uint32_t codepoint) const noexcept;
		/// Get a pointer to the page at the given index.
		[[nodiscard]] Core::Optional<const Page*> GetPage(PageIndex pageIndex) const noexcept;
		/// Returns the number of pages in this font map.
		[[nodiscard]] PageIndex GetPageCount() const noexcept;

	private:
		/// The size of the pages to use.
		Core::Math::Vec2u                mPageSize;
		/// The pages.
		std::vector<Page>                mPages;
		/// The addresses of all the glyphs in this font face.
		std::map<char32_t, GlyphAddress> mGlyphs;
	};
}
