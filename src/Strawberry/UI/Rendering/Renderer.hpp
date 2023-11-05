#pragma once

#include "Strawberry/Graphics/Vulkan/Queue.hpp"
#include "Strawberry/Graphics/Vulkan/Framebuffer.hpp"
#include "Strawberry/Graphics/Vulkan/RenderPass.hpp"
#include "Strawberry/Graphics/Vulkan/Pipeline.hpp"


namespace Strawberry::UI
{
	class Pane;


	class Renderer
	{
	public:
		Renderer(const Graphics::Vulkan::Queue& queue, Core::Math::Vec2u renderSize);


		void Render(const Pane& pane);


		Graphics::Vulkan::Framebuffer GetFramebuffer();


	protected:
		static Graphics::Vulkan::RenderPass CreateRenderPass(const Graphics::Vulkan::Queue& queue);
		static Graphics::Vulkan::Pipeline
		CreateRectanglePipeline(const Graphics::Vulkan::RenderPass& renderPass, Core::Math::Vec2u renderSize);


	private:
		Core::ReflexivePointer<Graphics::Vulkan::Queue> mQueue;
		Core::Math::Vec2u mRenderSize;
		Graphics::Vulkan::RenderPass mRenderPass;
		Graphics::Vulkan::Framebuffer mFramebuffer;
		Graphics::Vulkan::Pipeline mRectanglePipeline;
		Graphics::Vulkan::Buffer mRectanglePipelineVertexUniform;
	};
}