#include "Renderer.hpp"
#include "Strawberry/UI/Pane.hpp"
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(const Graphics::Vulkan::Queue& queue, Core::Math::Vec2u renderSize)
		: mQueue(queue)
		, mCommandBuffer(queue)
		, mRenderSize(renderSize)
		, mRenderPass(CreateRenderPass(queue))
		, mFramebuffer(mRenderPass, mRenderSize)
		, mRectanglePipeline(CreateRectanglePipeline(mRenderPass, renderSize))
		, mRectanglePipelineVertexUniform(*queue.GetDevice(), sizeof(Core::Math::Mat4f), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
	{
		Core::IO::DynamicByteBuffer vertexConstants;
		Core::Math::Mat4f viewMatrix = Core::Math::Translate(Core::Math::Vec3f(-1.0, -1.0, 0.0))
				* Core::Math::Scale(renderSize.AsType<float>().WithAdditionalValues(1.0f).Map([](float x) { return 2.0f / x; }));
		vertexConstants.Push(viewMatrix);
		mRectanglePipelineVertexUniform.SetData(vertexConstants);
		mRectanglePipeline.SetUniformBuffer(mRectanglePipelineVertexUniform, 0, 0);

		BeginRenderPass();
	}


	void Renderer::Render(const Pane& pane)
	{
		Core::IO::DynamicByteBuffer vertexPushConstants;
		vertexPushConstants.Push(pane.GetPosition());
		vertexPushConstants.Push(pane.GetSize());
		vertexPushConstants.Push(pane.GetFillColor());

		mCommandBuffer.PushConstants(mRectanglePipeline, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vertexPushConstants, 0);
		mCommandBuffer.Draw(4);
	}


	Graphics::Vulkan::Framebuffer Renderer::GetFramebuffer()
	{
		EndRenderPass();
		auto result = std::exchange(mFramebuffer, Graphics::Vulkan::Framebuffer(mRenderPass, mRenderSize));
		BeginRenderPass();
		return result;
	}


	Graphics::Vulkan::RenderPass Renderer::CreateRenderPass(const Graphics::Vulkan::Queue& queue)
	{
		return Graphics::Vulkan::RenderPass::Builder(*queue.GetDevice())
			.WithColorAttachment(VK_FORMAT_R32G32B32A32_SFLOAT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
			.WithSubpass(Graphics::Vulkan::SubpassDescription()
				.WithColorAttachment(0))
			.Build();
	}


	Graphics::Vulkan::Pipeline
	Renderer::CreateRectanglePipeline(const Graphics::Vulkan::RenderPass& renderPass, Core::Math::Vec2u renderSize)
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


	void Renderer::BeginRenderPass()
	{
		mCommandBuffer.Begin(true);
		mCommandBuffer.BindPipeline(mRectanglePipeline);
		mCommandBuffer.BindDescriptorSet(mRectanglePipeline, 0);
		mCommandBuffer.BeginRenderPass(mRenderPass, mFramebuffer);
	}


	void Renderer::EndRenderPass()
	{
		mCommandBuffer.EndRenderPass();
		mCommandBuffer.End();
		mQueue->Submit(mCommandBuffer);
	}
}
