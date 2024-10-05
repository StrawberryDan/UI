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
		FontMap(FontFace& fontFace, Core::Math::Vec2u pageSize);


		Core::Math::Vec2u GetBoundingBoxSize() const noexcept
		{
			return mMaxGlyphSize;
		}

	private:
		struct GlyphView
		{
			// In multiples of mMaxGlyphSize.
			Core::Math::Vec2u offset;
			// In pixels.
			Core::Math::Vec2u extent;
		};


		struct Page
		{
			std::map<char32_t, GlyphView> mGlyphs;
			Core::Image<Core::PixelGreyscale> mAtlas;
		};


		Core::Math::Vec2u mPageSize;
		Core::Math::Vec2u mMaxGlyphSize;
		Core::Math::Vec2u mGlyphsPerPage;

		std::vector<Page> mPages;
	};
}
