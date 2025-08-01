#include "TextNodeRenderer.hpp"


namespace Strawberry::UI
{
	void TextNodeRenderer::Submit(uint32_t drawIndex, const TextNode& node)
	{
	}

	void TextNodeRenderer::Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix)
	{
	}

	void TextNodeRenderer::LoadFont(std::string ID, GPUFontMap&& fontMap)
	{
		mFontMaps.insert_or_assign(ID, std::move(fontMap));
	}
}