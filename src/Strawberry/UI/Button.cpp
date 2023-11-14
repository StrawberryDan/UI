//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Button.hpp"
#include "CallbackEventListener.hpp"
#include "Rendering/Renderer.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/Event.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Button::Button(Graphics::Vulkan::Image& image)
		: mImage(image)
	{
		SetLocalSize(image.GetSize().AsType<float>().AsSize<2>());


		// Setup event listener
		AddEventListener(CallbackEventListener(
			[](const Graphics::Window::Event& event) {
				if (auto button = event.Value<Graphics::Window::Events::MouseButton>())
				{
					return button->action == Graphics::Input::KeyAction::Press && button->button == Graphics::Input::MouseButton::Left;
				}

				return false;
			},
			[this](const Graphics::Window::Event& event) {
				if (mCallback)
					mCallback();
				return false;
			}));
	}


	Button::Button(Graphics::Vulkan::Image& image, Button::Callback callback)
		: Button(image)
	{
		SetCallback(std::move(callback));
	}


	void Button::Render(Renderer& renderer)
	{
		if (GetVisible())
		{
			renderer.Render(*this);

			Node::Render(renderer);
		}
	}


	void Button::SetCallback(Button::Callback callback)
	{
		mCallback = std::move(callback);
	}
}