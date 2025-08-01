#pragma once
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Vulkan/CommandBuffer.hpp"
#include "Text/FontMap.hpp"


namespace Strawberry::UI
{
	class TextNode;

	class TextNodeRenderer
	{
	public:
		void Submit(uint32_t drawIndex, const TextNode& node);
		void Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix);


		void LoadFont(std::string ID, GPUFontMap&& fontMap);

	private:
		std::unordered_map<std::string, GPUFontMap> mFontMaps;
	};
}
