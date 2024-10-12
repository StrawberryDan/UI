#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/Core/Util/Image.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class GlyphBitmap
	{
	public:
		GlyphBitmap(uint32_t unicode, const Core::Image<Core::PixelGreyscale>& bitmap)
			: unicode(unicode)
			, mBitmap(bitmap) {}


		[[nodiscard]] uint32_t UnicodeCodepoint() const
		{
			return unicode;
		}


		[[nodiscard]] Core::Image<Core::PixelGreyscale> Bitmap() const
		{
			return mBitmap;
		}

	private:
		uint32_t                          unicode;
		Core::Image<Core::PixelGreyscale> mBitmap;
	};
}
