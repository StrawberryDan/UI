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


		std::u32string SetString(const std::u32string& string) noexcept;
		std::u32string GetString() const noexcept;


	private:
		std::shared_ptr<FontMap> mFontFace;
		std::u32string mGlyphs;
	};
}
