#include "Strawberry/UI/Rendering/ColoredNodeRenderer.hpp"
#include "RenderContext.hpp"
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/ColoredNode.hpp"
#include "Strawberry/Vulkan/Pipeline/RenderPass.hpp"
#include "Strawberry/Vulkan/Resource/Framebuffer.hpp"
#include "Strawberry/Vulkan/Queue/CommandBuffer.hpp"


static const uint8_t VERTEX_SHADER_CODE[] =
{
#include "ColouredNodeRenderer.vert.bin"
};


static const uint8_t FRAGMENT_SHADER_CODE[] =
{
#include "ColoredNodeRenderer.frag.bin"
};


namespace Strawberry::UI
{
	ColoredNodeRenderer::ColoredNodeRenderer(
		Vulkan::Framebuffer& framebuffer,
		uint32_t subpassIndex,
		const Core::Math::Mat4f& projectionMatrix,
		Core::Math::Vec2f contentScale)
			: mColouredNodePipelineLayout(CreateColouredNodePipelineLayout(framebuffer.GetDevice()))
			, mColouredNodePipeline(CreateColouredNodePipeline(framebuffer, mColouredNodePipelineLayout, subpassIndex))
			, mProjectionMatrix(projectionMatrix)
			, mContentScale(contentScale)
			, mDescriptorPool(framebuffer.GetDevice(), 0, 1, {
								VkDescriptorPoolSize{ .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1 }
							})
			, mRenderConstantsDescriptorSet(mDescriptorPool, mColouredNodePipelineLayout.GetSetLayout(0))
			, mRenderConstantsBuffer(
								   Vulkan::Buffer::Builder(framebuffer.GetDevice(),
														   Vulkan::MemoryTypeCriteria::HostVisible())
								   .WithData(Core::IO::DynamicByteBuffer::FromObjects(mProjectionMatrix))
								   .WithUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
								   .Build())

	{
		mRenderConstantsDescriptorSet.SetUniformBuffer(0, 0, mRenderConstantsBuffer);
	}


	Vulkan::Batch ColoredNodeRenderer::MakeBatch(const ColoredNode& node, const RenderContext& renderContext)
	{
		auto position = renderContext.position;
		position[0] *= mContentScale[0];
		position[1] *= mContentScale[1];

		auto extent = node.GetExtent();
		extent[0] *= mContentScale[0];
		extent[1] *= mContentScale[1];


		Vulkan::Batch batch(mColouredNodePipeline);
		batch.WithVertexCount(6);

		batch.WithVertexBuffer(
			0,
			Vulkan::Buffer::Builder(mColouredNodePipeline.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
				.WithData(
					Core::IO::DynamicByteBuffer::FromObjects(
						position,
						extent,
						node.GetColor()))
				.WithUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
				.Build());

		batch.WithDescriptorSet(0, &mRenderConstantsDescriptorSet);

		return std::move(batch);
	}


	Vulkan::PipelineLayout ColoredNodeRenderer::CreateColouredNodePipelineLayout(Vulkan::Device& device)
	{
		return Vulkan::PipelineLayout::Builder(device)
			   .WithDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			   .Build();
	}


	Vulkan::GraphicsPipeline ColoredNodeRenderer::CreateColouredNodePipeline(
		Vulkan::Framebuffer&    frameBuffer,
		Vulkan::PipelineLayout& pipelineLayout,
		uint32_t                subpassIndex)
	{
		Vulkan::Device&     device     = frameBuffer.GetDevice();
		Vulkan::RenderPass& renderPass = frameBuffer.GetRenderPass();
		return Vulkan::GraphicsPipeline::Builder(pipelineLayout, renderPass, subpassIndex)
			   .WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
			   .WithInputBinding(0, VK_VERTEX_INPUT_RATE_INSTANCE)
			   .WithInputAttribute(0, 0, sizeof(RenderQueueEntry::position), VK_FORMAT_R32G32_SFLOAT)
			   .WithInputAttribute(1, 0, sizeof(RenderQueueEntry::extent), VK_FORMAT_R32G32_SFLOAT)
			   .WithInputAttribute(2, 0, sizeof(RenderQueueEntry::color), VK_FORMAT_R32G32B32A32_SFLOAT)
			   .WithViewport(frameBuffer, false)
			   .WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE)
			   .WithAlphaColorBlending()
			   .WithShaderStage(VK_SHADER_STAGE_VERTEX_BIT,
								Vulkan::Shader::Compile(device, VERTEX_SHADER_CODE).Unwrap())
			   .WithShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT,
								Vulkan::Shader::Compile(device, FRAGMENT_SHADER_CODE).Unwrap())
			   .Build();
	}
}
