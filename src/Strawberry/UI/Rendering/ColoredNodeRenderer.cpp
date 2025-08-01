#include "ColoredNodeRenderer.hpp"

#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/ColoredNode.hpp"
#include "Strawberry/Vulkan/RenderPass.hpp"


static const uint8_t VERTEX_SHADER_CODE[] =
{
#include "NodeRenderer.vert.bin"
};


static const uint8_t FRAGMENT_SHADER_CODE[] =
{
#include "NodeRenderer.frag.bin"
};


namespace Strawberry::UI
{
	ColoredNodeRenderer::ColoredNodeRenderer(Vulkan::Framebuffer& framebuffer, size_t subpassIndex, Vulkan::FallbackChainAllocator<Vulkan::FreeListAllocator>& vertexBufferAllocator)
		: mColouredNodePipelineLayout(CreateColouredNodePipelineLayout(*framebuffer.GetDevice()))
		, mColouredNodePipeline(CreateColouredNodePipeline(framebuffer, mColouredNodePipelineLayout, subpassIndex))
		, mInputBuffer(vertexBufferAllocator, 1024 * 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
		, mDescriptorPool(*framebuffer.GetDevice(), 0, 1, {VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1}})
		, mRenderConstantsDescriptorSet(mDescriptorPool, mColouredNodePipelineLayout.GetSetLayout(0))
		, mRenderConstantsBuffer(vertexBufferAllocator, sizeof(Core::Math::Mat4f), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)

	{
		mRenderConstantsDescriptorSet.SetUniformBuffer(0, 0, mRenderConstantsBuffer);
	}

	void ColoredNodeRenderer::Submit(uint32_t drawIndex, const ColoredNode& node)
	{
		mEntries.emplace_back(
			ColouredNodeEntry
			{
				.drawIndex = drawIndex,
				.position = node.GetPosition(),
				.extent = node.GetExtent(),
				.color = node.GetColor(),
			});
	}

	void ColoredNodeRenderer::Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix)
	{
		mRenderConstantsBuffer.SetData(Core::IO::DynamicByteBuffer(projectionMatrix));
		mInputBuffer.SetData({mEntries.data(), mEntries.size() * sizeof(ColouredNodeEntry)});
		commandBuffer.BindPipeline(mColouredNodePipeline);
		commandBuffer.BindVertexBuffer(0, mInputBuffer);
		commandBuffer.BindDescriptorSet(mColouredNodePipeline, 0, mRenderConstantsDescriptorSet);
		commandBuffer.Draw(6, mEntries.size());
		mEntries.clear();
	}

	Vulkan::PipelineLayout ColoredNodeRenderer::CreateColouredNodePipelineLayout(Vulkan::Device& device)
	{
		return Vulkan::PipelineLayout::Builder(device)
			.WithDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.WithDescriptor(1, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, 256)
			.Build();
	}

	Vulkan::GraphicsPipeline ColoredNodeRenderer::CreateColouredNodePipeline(
		Vulkan::Framebuffer& frameBuffer,
		Vulkan::PipelineLayout& pipelineLayout,
		size_t subpassIndex)
	{
		Vulkan::Device& device = *frameBuffer.GetDevice();
		Vulkan::RenderPass& renderPass = *frameBuffer.GetRenderPass();
		return Vulkan::GraphicsPipeline::Builder(pipelineLayout, renderPass, subpassIndex)
			.WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
			.WithInputBinding(0, sizeof(ColouredNodeEntry), VK_VERTEX_INPUT_RATE_INSTANCE)
			.WithInputAttribute(0, 0, offsetof(ColouredNodeEntry, ColouredNodeEntry::drawIndex), VK_FORMAT_R32_UINT)
			.WithInputAttribute(1, 0, offsetof(ColouredNodeEntry, ColouredNodeEntry::position), VK_FORMAT_R32G32_SFLOAT)
			.WithInputAttribute(2, 0, offsetof(ColouredNodeEntry, ColouredNodeEntry::extent), VK_FORMAT_R32G32_SFLOAT)
			.WithInputAttribute(3, 0, offsetof(ColouredNodeEntry, ColouredNodeEntry::color), VK_FORMAT_R32G32B32A32_SFLOAT)
			.WithViewport(frameBuffer, false)
			.WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE)
			.WithAlphaColorBlending()
			.WithDepthTesting()
			.WithShaderStage(VK_SHADER_STAGE_VERTEX_BIT, Vulkan::Shader::Compile(device, VERTEX_SHADER_CODE).Unwrap())
			.WithShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, Vulkan::Shader::Compile(device, FRAGMENT_SHADER_CODE).Unwrap())
			.Build();
	}
}
