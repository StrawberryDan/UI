
#include "Strawberry/UI/Rendering/Renderer.hpp"
#include <Strawberry/Graphics/Window.hpp>
#include "Strawberry/Graphics/Vulkan/Instance.hpp"
#include "Strawberry/Graphics/Vulkan/Device.hpp"
#include "Strawberry/Graphics/Vulkan/Queue.hpp"
#include "Strawberry/Graphics/Vulkan/Swapchain.hpp"
#include "Strawberry/UI/Pane.hpp"
#include "Strawberry/UI/Frame.hpp"
#include "Strawberry/UI/CallbackEventListener.hpp"
#include "Strawberry/Core/IO/Logging.hpp"
#include "Strawberry/Graphics/2D/SpriteSheet.hpp"


using namespace Strawberry;


int main()
{
	Graphics::FreeType::Initialise();

	Graphics::Window::Window window("StrawberryUI Test", Core::Math::Vec2i(1920, 1080));
	Graphics::Vulkan::Instance instance;
	auto device = instance.Create<Graphics::Vulkan::Device>();
	auto surface = window.Create<Graphics::Vulkan::Surface>(device);
	auto queue = device.Create<Graphics::Vulkan::Queue>();
	auto swapchain = queue.Create<Graphics::Vulkan::Swapchain>(surface, Core::Math::Vec2i(1920, 1080));

	UI::Renderer uiRenderer(queue, Core::Math::Vec2u(1920, 1080));

	Graphics::FontFace font = Graphics::FontFace::FromFile("data/Pixels.ttf").Unwrap();

	Graphics::Vulkan::Image cassius = Graphics::Vulkan::Image::FromFile(queue, "data/cassius.png").Unwrap();

	UI::Pane pane;
	pane.SetLocalPosition({40, 40});
	pane.SetLocalSize({167 + 20, 184 + 20});
	pane.AddEventListener(UI::CallbackEventListener(
			[](const Graphics::Window::Event& x){ return x.IsType<Graphics::Window::Events::MouseButton>(); },
			[](const auto& event) {
				Core::Logging::Info("Square!"); return false;
			}));

	auto subpane = pane.AppendChild<UI::Text>(font);
	subpane->SetLocalPosition({190, 0});
	subpane->SetLocalSize({300, 50});
	subpane->SetFontColor({1.0, 1.0, 1.0, 1.0});
	subpane->SetString("Help me! Those Capua Sky Bandits have trapped me in this Vulkan UI Rendering Example.");
	subpane->SetFontSize(75);

	auto imagepane = pane.AppendChild<UI::Image>(cassius);
	imagepane->SetLocalPosition({10, 10});
	imagepane->AddEventListener(UI::CallbackEventListener(
			[](const Graphics::Window::Event& x){ return x.IsType<Graphics::Window::Events::MouseButton>(); },
			[](const auto& event) {
				Core::Logging::Info("Cassius!!"); return false;
			}));

	// Load Estelle Image
	auto donImg = Graphics::SpriteSheet(queue, Graphics::Vulkan::Image::FromFile(queue, "data/don_capua.png").Unwrap(), Core::Math::Vec2u(2, 1));
	auto don = imagepane->AppendChild<UI::Sprite>(donImg);
	don->SetLocalPosition({200, 200});
	don->AddEventListener(UI::CallbackEventListener(
		[](const Graphics::Window::Event& x) {
			if (auto button = x.Value<Graphics::Window::Events::MouseButton>())
			{
				return button->button == Graphics::Input::MouseButton::Left
					&& button->action == Graphics::Input::KeyAction::Press;
			}

			return false;
		},
		[&](const auto& event) { don->GetSprite().SetSpriteIndex(don->GetSprite().GetSpriteIndex() + 1); return false; }));


	UI::Frame frame(std::move(pane));

	while(!window.CloseRequested())
	{
		Graphics::Window::PollInput();
		while (auto event = window.NextEvent())
		{
			frame.Dispatch(event.Value());
		}

		frame.Render(uiRenderer);
		auto framebuffer = uiRenderer.TakeFramebuffer();

		swapchain.Present(framebuffer);
		window.SwapBuffers();
	}

	Graphics::FreeType::Terminate();

	return 0;
}
