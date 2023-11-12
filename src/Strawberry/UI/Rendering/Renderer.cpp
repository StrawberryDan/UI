#include "Renderer.hpp"
#include "Strawberry/UI/Pane.hpp"
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Graphics::Vulkan::Queue& queue, Core::Math::Vec2u resolution)
		: Graphics::Renderer(queue, CreateRenderPass(queue), resolution)
		, mRectangleRenderer(*GetQueue(), mRenderPass.Get(), GetResolution())
		, mTextRenderer(*GetQueue(), mRenderPass.Get(), GetResolution())
	{

	}


	void Renderer::Render(const Pane& pane)
	{
		mRectangleRenderer.SetFramebuffer(TakeFramebuffer());
		mRectangleRenderer.Render(pane);
		SetFramebuffer(mRectangleRenderer.TakeFramebuffer());
	}


	void Renderer::Render(const Text& text)
	{
		mTextRenderer.SetFramebuffer(TakeFramebuffer());
		mTextRenderer.Draw(text.GetFontFace(), text.GetString(), text.GetPosition().AsType<int>(), text.GetFontColor());
		SetFramebuffer(mTextRenderer.TakeFramebuffer());
	}


	Graphics::Vulkan::RenderPass& Renderer::CreateRenderPass(const Graphics::Vulkan::Queue& queue)
	{
		mRenderPass.Construct(Graphics::Vulkan::RenderPass::Builder(*queue.GetDevice())
			.WithColorAttachment(VK_FORMAT_R32G32B32A32_SFLOAT, VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE)
			.WithSubpass(Graphics::Vulkan::SubpassDescription()
								 .WithColorAttachment(0))
			.Build());
		return mRenderPass.Get();
	}
}
