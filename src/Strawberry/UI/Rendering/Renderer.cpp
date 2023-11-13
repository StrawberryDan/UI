#include "Renderer.hpp"
#include "Strawberry/UI/Pane.hpp"
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Graphics::Vulkan::Queue& queue, Core::Math::Vec2u resolution)
		: Graphics::Renderer(queue, resolution)
		, mRectangleRenderer(*GetQueue(), GetResolution())
		, mTextRenderer(*GetQueue(), GetResolution())
		, mImageRenderer(*GetQueue(), GetResolution())
	{

	}


	void Renderer::Render(const Pane& pane)
	{
		if (FramebufferAvailable())
			mRectangleRenderer.SetFramebuffer(TakeFramebuffer());
		mRectangleRenderer.Render(pane);
		SetFramebuffer(mRectangleRenderer.TakeFramebuffer());
	}


	void Renderer::Render(const Text& text)
	{
		if (FramebufferAvailable())
			mTextRenderer.SetFramebuffer(TakeFramebuffer());
		mTextRenderer.Draw(text.GetFontFace(), text.GetString(), text.GetPosition(), text.GetFontColor());
		SetFramebuffer(mTextRenderer.TakeFramebuffer());
	}


	void Renderer::Render(const Image& image)
	{
		// Return early if the image has no texture assigned yet.
		if (!image.GetImage()) return;

		if (FramebufferAvailable())
			mImageRenderer.SetFramebuffer(TakeFramebuffer());
		mImageRenderer.Draw(*image.GetImage(), image.GetPosition(), image.GetSize());
		SetFramebuffer(mImageRenderer.TakeFramebuffer());
	}
}
