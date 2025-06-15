#include "NodeRenderer.hpp"

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
	NodeRenderer::NodeRenderer(Vulkan::Framebuffer& framebuffer, size_t subpassIndex)
		: mProjectionMatrix(CreateProjectionMatrix(framebuffer))
		, mPipelineLayout(CreatePipelineLayout(*framebuffer.GetDevice()))
		, mPipeline(CreatePipeline(framebuffer, mPipelineLayout, subpassIndex))
		, mAllocator(Vulkan::MemoryPool::Allocate(
			*framebuffer.GetDevice(),
			framebuffer.GetDevice()->GetPhysicalDevice().SearchMemoryTypes(Vulkan::MemoryTypeCriteria::HostVisible())[0].index,
			1024 * 1024 * 1024
			).Unwrap())
		, mInputBuffer(mAllocator, 1024 * 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
		, mDescriptorPool(*framebuffer.GetDevice(), 0, 1, {VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1}})
		, mRenderConstantsDescriptorSet(mDescriptorPool, mPipelineLayout.GetSetLayout(0))
		, mRenderConstantsBuffer(mAllocator, sizeof(Core::Math::Mat4f), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
	{
		mRenderConstantsDescriptorSet.SetUniformBuffer(0, 0, mRenderConstantsBuffer);
	}


	void NodeRenderer::Submit(const Node& node)
	{
		node.Render(*this);
	}

	void NodeRenderer::SubmitColouredNode(const ColoredNode& node)
	{
		mEntries.emplace_back(
			Entry
			{
				.position = node.GetPosition(),
				.extent = node.GetExtent(),
				.color = node.GetColor(),
			});
	}

	void NodeRenderer::Render(Vulkan::CommandBuffer& commandBuffer)
	{
		mRenderConstantsBuffer.SetData(Core::IO::DynamicByteBuffer(mProjectionMatrix));
		mInputBuffer.SetData({mEntries.data(), mEntries.size() * sizeof(Entry)});
		commandBuffer.BindPipeline(mPipeline);
		commandBuffer.BindVertexBuffer(0, mInputBuffer);
		commandBuffer.BindDescriptorSet(mPipeline, 0, mRenderConstantsDescriptorSet);
		commandBuffer.Draw(6, mEntries.size());
		mEntries.clear();
	}

	Core::Math::Mat4f NodeRenderer::CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer)
	{
		return
			Core::Math::Translate<float>(-1.0f, -1.0f, 0.0f) *
			Core::Math::Scale<float>(1.0f / framebuffer.GetSize()[0], 1.0f / framebuffer.GetSize()[1], 1.0f);
	}

	Vulkan::PipelineLayout NodeRenderer::CreatePipelineLayout(Vulkan::Device& device)
	{
		return Vulkan::PipelineLayout::Builder(device)
			.WithDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.Build();
	}

	Vulkan::GraphicsPipeline NodeRenderer::CreatePipeline(
		Vulkan::Framebuffer& frameBuffer,
		Vulkan::PipelineLayout& pipelineLayout,
		size_t subpassIndex)
	{
		Vulkan::Device& device = *frameBuffer.GetDevice();
		Vulkan::RenderPass& renderPass = *frameBuffer.GetRenderPass();
		return Vulkan::GraphicsPipeline::Builder(pipelineLayout, renderPass, subpassIndex)
			.WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
			.WithInputBinding(0, sizeof(Entry), VK_VERTEX_INPUT_RATE_INSTANCE)
			.WithInputAttribute(0, 0, offsetof(Entry, Entry::position), VK_FORMAT_R32G32_SFLOAT)
			.WithInputAttribute(1, 0, offsetof(Entry, Entry::extent), VK_FORMAT_R32G32_SFLOAT)
			.WithInputAttribute(2, 0, offsetof(Entry, Entry::color), VK_FORMAT_R32G32B32A32_SFLOAT)
			.WithViewport(frameBuffer, false)
			.WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE)
			.WithAlphaColorBlending()
			.WithShaderStage(VK_SHADER_STAGE_VERTEX_BIT, Vulkan::Shader::Compile(device, VERTEX_SHADER_CODE).Unwrap())
			.WithShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, Vulkan::Shader::Compile(device, FRAGMENT_SHADER_CODE).Unwrap())
			.Build();
	}
}
