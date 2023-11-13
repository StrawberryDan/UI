#pragma once

#include "Strawberry/UI/Text.hpp"
#include "RectangleRenderer.hpp"
#include "Strawberry/Graphics/Renderer.hpp"
#include "Strawberry/Graphics/2D/TextRenderer.hpp"
#include "Strawberry/Core/Types/Delayed.hpp"
#include "Strawberry/Graphics/2D/ImageRenderer.hpp"
#include "Strawberry/UI/Image.hpp"


namespace Strawberry::UI
{
	class Pane;


	class Renderer
		: public Graphics::Renderer
	{
	public:
		Renderer(Graphics::Vulkan::Queue& queue, Core::Math::Vec2u resolution);


		void Render(const Pane& pane);
		void Render(const Text& text);
		void Render(const Image& image);


	private:
		RectangleRenderer mRectangleRenderer;
		Graphics::TextRenderer mTextRenderer;
		Graphics::ImageRenderer mImageRenderer;
	};
}
