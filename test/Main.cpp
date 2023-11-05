
#include "Strawberry/UI/Rendering/Renderer.hpp"
#include <Strawberry/Graphics/Window.hpp>
#include "Strawberry/Graphics/Vulkan/Instance.hpp"
#include "Strawberry/Graphics/Vulkan/Device.hpp"
#include "Strawberry/Graphics/Vulkan/Queue.hpp"
#include "Strawberry/Graphics/Vulkan/Swapchain.hpp"
#include "../src/Strawberry/UI/Pane.hpp"


using namespace Strawberry;


int main()
{
	Graphics::Window::Window window("StrawberryUI Test", Core::Math::Vec2i(1920, 1080));
	Graphics::Vulkan::Instance instance;
	auto device = instance.Create<Graphics::Vulkan::Device>();
	auto surface = window.Create<Graphics::Vulkan::Surface>(device);
	auto queue = device.Create<Graphics::Vulkan::Queue>();
	auto swapchain = queue.Create<Graphics::Vulkan::Swapchain>(surface, Core::Math::Vec2i(1920, 1080));

	UI::Renderer uiRenderer(queue, Core::Math::Vec2u(1920, 1080));

	std::unique_ptr<UI::Pane> pane  = std::make_unique<UI::Pane>();
	pane->SetLocalPosition({40, 40});
	pane->SetLocalSize({100, 100});

	std::unique_ptr<UI::Pane> subpane = std::make_unique<UI::Pane>();
	subpane->SetLocalPosition({110, 0});
	subpane->SetLocalSize({50, 50});
	pane->AppendChild(std::move(subpane));

	while(!window.CloseRequested())
	{
		Graphics::Window::PollInput();
		while (auto event = window.NextEvent())
		{

		}

		pane->Render(uiRenderer);
		auto framebuffer = uiRenderer.GetFramebuffer();

		swapchain.Present(framebuffer);
		window.SwapBuffers();
	}
	return 0;
}
