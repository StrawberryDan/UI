#pragma once

#include "Strawberry/Graphics/Vulkan/Queue.hpp"
#include "Strawberry/Graphics/Vulkan/Framebuffer.hpp"
#include "Strawberry/Graphics/Vulkan/RenderPass.hpp"
#include "Strawberry/Graphics/Vulkan/Pipeline.hpp"
#include "Strawberry/Graphics/Vulkan/CommandBuffer.hpp"
#include "Strawberry/UI/Text.hpp"
#include <Strawberry/Core/Types/Optional.hpp>
#include <Strawberry/Graphics/Text/TextRenderer.hpp>


namespace Strawberry::UI
{
	class Pane;


	class Renderer
	{
	public:
		Renderer(const Graphics::Vulkan::Queue& queue, Core::Math::Vec2u renderSize);


		void Render(const Pane& pane);
		void Render(const Text& text);


		void SetFramebuffer(Graphics::Vulkan::Framebuffer framebuffer);
		Graphics::Vulkan::Framebuffer GetFramebuffer();


	protected:
		static Graphics::Vulkan::RenderPass CreateRenderPass(const Graphics::Vulkan::Queue& queue);
		static Graphics::Vulkan::Pipeline CreateRectanglePipeline(const Graphics::Vulkan::RenderPass& renderPass,
																  Core::Math::Vec2u renderSize);

		void BeginRenderPass();
		void EndRenderPass();


	private:
		Core::ReflexivePointer<Graphics::Vulkan::Queue> mQueue;
		Graphics::Vulkan::CommandBuffer mCommandBuffer;
		Core::Math::Vec2u mRenderSize;
		Graphics::Vulkan::RenderPass mRenderPass;
		Core::Optional<Graphics::Vulkan::Framebuffer> mFramebuffer;

		Graphics::Vulkan::Pipeline mRectanglePipeline;
		Graphics::Vulkan::DescriptorSet mRectanglePipelineVertexShaderDescriptorSet;
		Graphics::Vulkan::Buffer mRectanglePipelineVertexShaderUniformBuffer;

		Graphics::TextRenderer mTextRenderer;
	};
}
