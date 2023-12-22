#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "RectangularNode.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/2D/FontFace.hpp"
// Lib Format
#include <fmt/format.h>
// Standard Library
#include <string>
#include <memory>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Text
		: public RectangularNode
	{
	public:
		Text(const Graphics::FontFace& fontFace);
		Text(const Graphics::FontFace& fontFace, unsigned int fontSize);
		Text(const Graphics::FontFace& fontFace, unsigned int fontSize, const std::string& string);
		Text(const Graphics::FontFace& fontFace, unsigned int fontSize, const std::u32string& string);


		template<typename ...Args>
		constexpr Text(const Graphics::FontFace& fontFace, unsigned int fontSize, const std::string& string, Args... args)
			: Text(fontFace, fontSize, fmt::format(fmt::runtime(string), args...))
		{}


		void Render(Renderer& renderer) override;


		Graphics::FontFace& GetFontFace() const;
		void SetFontFace(const Graphics::FontFace& face);


		const Core::Math::Vec4f& GetFontColor() const;
		void SetFontColor(const Core::Math::Vec4f color);


		uint32_t GetFontSize() const;
		void SetFontSize(uint32_t fontSize);


		const std::u32string& GetString() const;
		void SetString(const std::u32string& string);
		void SetString(const std::u8string& string);
		void SetString(const std::string& string);


		template<typename ...Args>
		void SetString(const std::string& string, Args... args)
		{
			SetString(fmt::format(fmt::runtime(string), args...));
		}


	protected:
		void UpdateSize();


	private:
		std::u32string mString;
		Core::ReflexivePointer<Graphics::FontFace> mFontFace;
		uint32_t mFontSize;
		Core::Math::Vec4f mFontColor = Core::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	};
}
