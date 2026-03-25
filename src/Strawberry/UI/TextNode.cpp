#include "TextNode.hpp"

#include "Rendering/ColoredNodeRenderer.hpp"
#include "Rendering/Renderer.hpp"
#include "Strawberry/Core/UTF.hpp"


namespace Strawberry::UI
{
	std::u32string TextNode::SetString(const std::u32string& string) noexcept
	{
		return mGlyphs = string;
	}

	std::u32string TextNode::GetString() const noexcept
	{
		return mGlyphs;
	}
}
