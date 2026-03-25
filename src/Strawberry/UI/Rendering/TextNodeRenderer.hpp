#pragma once
#include "Strawberry/UI/Rendering/RenderBatcher.hpp"
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
		struct RenderQueueEntry
		{
			Core::Math::Vec2f position;
			Core::Math::Vec2f extent;
			uint32_t glyphAddressPageIndex;
			Core::Math::Vec2u glyphAddressCoordinate;
			Core::Math::Vec2u glyphAddressExtent;

			std::string_view font;
		};


		TextNodeRenderer(Vulkan::Framebuffer& framebuffer, unsigned int subpassIndex, const Core::Math::Mat4f projectionMatrix, Core::Math::Vec2f contentScale);

		RenderBatcher::Batch MakeBatch(const TextNode& node);

		void RenderBatch(const Vulkan::Buffer& batch) const;

		void LoadFont(Vulkan::Queue& queue, std::string ID, FontFace&& fontFace);

	private:
		Core::Math::Vec2u mPageSize = {4096, 4096};

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


		std::vector<Glyph>    mGlyphBuffer;
		Vulkan::Buffer        mGPUGlyphBuffer;
		Vulkan::DescriptorSet mGlyphBufferDescriptorSet;

		Core::Math::Vec2f     mContentScale;
		Vulkan::Buffer        mConstantsBuffer;
	};
}
