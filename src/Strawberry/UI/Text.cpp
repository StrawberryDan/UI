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


	void Text::Render(Strawberry::UI::Renderer& renderer)
	{
		renderer.Render(*this);
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
		mFontFace->SetPixelSize(mFontSize);
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
	}


	void Text::SetString(const std::u8string& string)
	{
		SetString(Core::ToUTF32(string));
	}


	void Text::SetString(const std::string& string)
	{
		SetString(std::u8string(string.begin(), string.end()));
	}
}
