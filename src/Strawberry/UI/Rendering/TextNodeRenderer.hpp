#pragma once
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Vulkan/Queue/CommandBuffer.hpp"
#include "Strawberry/Vulkan/Pipeline/PipelineLayout.hpp"
#include "Strawberry/Vulkan/Pipeline/GraphicsPipeline.hpp"
#include "Strawberry/Vulkan/Resource/Buffer.hpp"
#include "Text/FontMap.hpp"


namespace Strawberry::UI
{
	class TextNode;

	class TextNodeRenderer
	{
	public:
		TextNodeRenderer(Vulkan::Framebuffer& framebuffer, unsigned int subpassIndex);


		void Submit(uint32_t drawIndex, const TextNode& node);

		void Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix);


		void LoadFont(Vulkan::Queue& queue, std::string ID, FontFace&& fontFace);

	private:
		struct LoadedFont
		{
			FontFace              fontFace;
			FontMap               cpuFontMap;
			GPUFontMap            gpuFontMap;
			Vulkan::DescriptorSet descriptorSet;
		};

		std::unordered_map<std::string, LoadedFont> mFontMaps;

		Vulkan::PipelineLayout   mPipelineLayout;
		Vulkan::GraphicsPipeline mGraphicsPipeline;


		struct Glyph
		{
			std::string       font;
			Core::Math::Vec2u position;
			Core::Math::Vec2u size;
			uint32_t          glyphAddressPageIndex;
			Core::Math::Vec2u glyphAddressCoordinate;
		};


		std::vector<Glyph>    mGlyphBuffer;
		Vulkan::Buffer        mGPUGlyphBuffer;
		Vulkan::DescriptorSet mGlyphBufferDescriptorSet;

		Vulkan::Buffer        mConstantsBuffer;
	};
}
