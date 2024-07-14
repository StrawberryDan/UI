#include "Strawberry/Core/IO/Logging.hpp"
#include "Strawberry/UI/Events/CallbackListener.hpp"
#include "Strawberry/UI/Frame.hpp"
#include "Strawberry/Vulkan/Instance.hpp"
#include "Strawberry/Vulkan/Device.hpp"
#include "Strawberry/Vulkan/Swapchain.hpp"
#include "Strawberry/Vulkan/Surface.hpp"
#include "Strawberry/Window/Window.hpp"
#include "Strawberry/Rendering/Text/FontFace.hpp"


using namespace Strawberry;


int main()
{
	Window::Window   window("StrawberryUI Test", Core::Math::Vec2i(1920, 1080));
	Vulkan::Instance instance;
	auto&            physicalDevice   = instance.GetPhysicalDevices()[0];
	auto             queueFamilyIndex = physicalDevice.SearchQueueFamilies(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT)[0];
	Vulkan::Device   device(physicalDevice,
	                      {
		                      Vulkan::QueueCreateInfo{.familyIndex = queueFamilyIndex, .count = 1}
	                      });
	Vulkan::Surface   surface(window, device);
	auto              queue = device.GetQueue(queueFamilyIndex, 0);
	Vulkan::Swapchain swapchain(*queue, surface, window.GetSize(), VK_PRESENT_MODE_FIFO_KHR);


	UI::FontFace fontFace = UI::FontFace::FromFile("data/Pixels.ttf").Unwrap();
	auto         glyph    = fontFace.LoadGlyph(0);


	while (!window.CloseRequested())
	{
		Window::PollInput();
		window.SwapBuffers();
	}

	return 0;
}
