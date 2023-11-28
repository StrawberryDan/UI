//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Text.hpp"
#include "Rendering/Renderer.hpp"
// Strawberry Core
#include <Strawberry/Core/UTF.hpp>


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Text::Text(const Graphics::FontFace& fontFace)
		: mFontFace(fontFace)
	{}


	Text::Text(const Graphics::FontFace& fontFace, unsigned fontSize)
		: Text(fontFace)
	{
		SetFontSize(fontSize);
	}


	Text::Text(const Graphics::FontFace& fontFace, unsigned fontSize, const std::string& string)
		: Text(fontFace, fontSize)
	{
		SetString(string);
	}


	Text::Text(const Graphics::FontFace& fontFace, unsigned fontSize, const std::u32string& string)
		: Text(fontFace, fontSize)
	{
		SetString(string);
	}


	void Text::Render(Strawberry::UI::Renderer& renderer)
	{
		if (IsVisible())
		{
			renderer.Render(*this);

			for (int i = 0; i < GetChildCount(); i++)
				GetChild(i)->Render(renderer);
		}
	}


	Graphics::FontFace& Text::GetFontFace() const
	{
		return *mFontFace;
	}


	void Text::SetFontFace(const Graphics::FontFace& face)
	{
		mFontFace = face.GetReflexivePointer();
	}


	uint32_t Text::GetFontSize() const
	{
		return mFontSize;
	}


	void Text::SetFontSize(uint32_t fontSize)
	{
		mFontSize = fontSize;
		UpdateSize();
	}


	const Core::Math::Vec4f& Text::GetFontColor() const
	{
		return mFontColor;
	}


	void Text::SetFontColor(const Core::Math::Vec4f color)
	{
		mFontColor = color;
	}


	const std::u32string& Text::GetString() const
	{
		return mString;
	}


	void Text::SetString(const std::u32string& string)
	{
		mString = string;
		UpdateSize();
	}


	void Text::SetString(const std::u8string& string)
	{
		SetString(Core::ToUTF32(string));
	}


	void Text::SetString(const std::string& string)
	{
		SetString(std::u8string(string.begin(), string.end()));
	}


	void Text::UpdateSize()
	{
		mFontFace->SetPixelSize(mFontSize);

		Core::Math::Vec2f newSize;
		for (auto c : GetString())
		{
			newSize[0] += mFontFace->GetGlyphAdvance(c)[0];
			newSize[1] = std::max(newSize[1], mFontFace->GetGlyphBoundingBox(c)[1]);
		}

		SetLocalSize(newSize);
	}
}
