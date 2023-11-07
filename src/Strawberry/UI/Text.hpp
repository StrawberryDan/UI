#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Pane.hpp"
// Strawberry Graphics
#include <Strawberry/Graphics/Text/FontFace.hpp>
// Standard Library
#include <string>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Text
		: public Pane
	{
	public:
		Text(const Graphics::FontFace& fontFace);


		void Render(Renderer& renderer) override;


		Graphics::FontFace& GetFontFace() const;
		void SetFontFace(const Graphics::FontFace& face);


		uint32_t GetFontSize() const;
		void SetFontSize(uint32_t fontSize);


		const std::u32string& GetString() const;
		void SetString(const std::u32string& string);
		void SetString(const std::u8string& string);
		void SetString(const std::string& string);


	private:
		std::u32string mString;
		Core::ReflexivePointer<Graphics::FontFace> mFontFace;
		uint32_t mFontSize;
	};
}
