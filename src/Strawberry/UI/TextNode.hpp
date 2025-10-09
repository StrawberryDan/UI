#pragma once
#include "Node.hpp"



namespace Strawberry::UI
{
	class FontMap;

	class TextNode
		: public Node
	{
	public:
		void Render(Renderer& renderer) const override;


		const std::string& GetFont() const { return mFontName; }
		void SetFont(const std::string& newFont) { mFontName = newFont; }


		std::u32string SetString(const std::u32string& string) noexcept;
		std::u32string GetString() const noexcept;


	private:
		std::string    mFontName;
		std::u32string mGlyphs;
	};
}
