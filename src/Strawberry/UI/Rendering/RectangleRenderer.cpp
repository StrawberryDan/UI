//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "RectangleRenderer.hpp"
#include "Strawberry/UI/Pane.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/Vulkan/CommandBuffer.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	RectangleRenderer::RectangleRenderer(Graphics::Vulkan::Queue& queue, Core::Math::Vec2u resolution)
		: Graphics::Renderer(queue, CreateRenderPass(*queue.GetDevice()), resolution)
		, mPipeline(CreatePipeline(*GetRenderPass(), GetResolution()))
		, mVertexDescriptorSet(mPipeline.AllocateDescriptorSet(0))
		, mVertexUniformBuffer(*GetQueue()->GetDevice(), sizeof(Core::Math::Mat4f), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
	{
		Core::IO::DynamicByteBuffer vertexConstants;
		Core::Math::Mat4f viewMatrix = Core::Math::Translate(Core::Math::Vec3f(-1.0, -1.0, 0.0))
									   * Core::Math::Scale(GetResolution().AsType<float>().WithAdditionalValues(1.0f).Map([](float x) { return 2.0f / x; }));
		vertexConstants.Push(viewMatrix);
		mVertexUniformBuffer.SetData(vertexConstants);
		mVertexDescriptorSet.SetUniformBuffer(mVertexUniformBuffer, 0, 0);
	}


	void RectangleRenderer::Render(const Pane& pane)
	{
		Core::IO::DynamicByteBuffer vertexPushConstants;
		vertexPushConstants.Push(pane.GetPosition());
		vertexPushConstants.Push(pane.GetSize());
		vertexPushConstants.Push(pane.GetFillColor());

		auto commandBuffer = GetQueue()->Create<Graphics::Vulkan::CommandBuffer>();
		commandBuffer.Begin(true);
		commandBuffer.BindPipeline(mPipeline);
		commandBuffer.BindDescriptorSet(mPipeline, 0, mVertexDescriptorSet);
		commandBuffer.BeginRenderPass(*GetRenderPass(), GetFramebuffer());
		commandBuffer.PushConstants(mPipeline, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vertexPushConstants, 0);
		commandBuffer.Draw(4);
		commandBuffer.EndRenderPass();
		commandBuffer.End();
		GetQueue()->Submit(std::move(commandBuffer));
	}


	Graphics::Vulkan::RenderPass RectangleRenderer::CreateRenderPass(const Graphics::Vulkan::Device& device)
	{
		return Graphics::Vulkan::RenderPass::Builder(device)
			.WithColorAttachment(VK_FORMAT_R32G32B32A32_SFLOAT, VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE)
			.WithSubpass(Graphics::Vulkan::SubpassDescription().WithColorAttachment(0))
			.Build();
	}


	Graphics::Vulkan::Pipeline
	RectangleRenderer::CreatePipeline(const Graphics::Vulkan::RenderPass& renderPass,
									  Core::Math::Vec2u renderSize)
	{
		static const uint8_t vertexShaderCode[] =
		{
			#include "Rectangle.vert.bin"
		};

		static const uint8_t fragmentShaderCode[] =
		{
			#include "Rectangle.frag.bin"
		};


		auto vertexShader = Graphics::Vulkan::Shader::Compile(*renderPass.GetDevice(), Core::IO::DynamicByteBuffer(vertexShaderCode, sizeof(vertexShaderCode))).Unwrap();
		auto fragmentShader = Graphics::Vulkan::Shader::Compile(*renderPass.GetDevice(), Core::IO::DynamicByteBuffer(fragmentShaderCode, sizeof(fragmentShaderCode))).Unwrap();


		return Graphics::Vulkan::Pipeline::Builder(renderPass)
				.WithPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP)
				.WithPushConstantRange(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 2 * sizeof(Core::Math::Vec2f) + sizeof(Core::Math::Vec4f), 0)
				.WithDescriptorSetLayout(Graphics::Vulkan::DescriptorSetLayout()
												 .WithBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT))
				.WithShaderStage(VK_SHADER_STAGE_VERTEX_BIT, std::move(vertexShader))
				.WithShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(fragmentShader))
				.WithViewport({0.0f, 0.0f}, renderSize.AsType<float>())
				.Build();
	}
}
