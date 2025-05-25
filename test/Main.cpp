#include "Strawberry/Core/IO/Logging.hpp"
#include "Strawberry/Vulkan/Instance.hpp"
#include "Strawberry/Vulkan/Device.hpp"
#include "Strawberry/Vulkan/Swapchain.hpp"
#include "Strawberry/Vulkan/Surface.hpp"
#include "Strawberry/Window/Window.hpp"
#include "Strawberry/UI/Rendering/Text/FontFace.hpp"
#include "Strawberry/UI/Rendering/Text/FontMap.hpp"
#include "Strawberry/Window/Monitor.hpp"


using namespace Strawberry;


int main()
{	ZoneScoped;
	Window::Window   window("StrawberryUI Test", Core::Math::Vec2i(1920, 1080));
	Vulkan::Instance instance;
	auto&            physicalDevice   = instance.GetPhysicalDevices()[0];
	auto             queueFamilyIndex = physicalDevice.SearchQueueFamilies(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT)[0];
	Vulkan::Device   device(physicalDevice, VkPhysicalDeviceFeatures{},
	                      {
		                      Vulkan::QueueCreateInfo{.familyIndex = queueFamilyIndex, .count = 1}
	                      });
	Vulkan::Surface   surface(window, device);
	Vulkan::Queue&    queue = device.GetQueue(queueFamilyIndex, 0);
	Vulkan::Swapchain swapchain(queue, surface, window.GetSize(), VK_PRESENT_MODE_FIFO_KHR);


	UI::FontFace fontFace = UI::FontFace::FromFile("data/italianno.ttf").Unwrap();
	fontFace.SetSizePoints(40, Window::GetMonitorInfo()[0].GetDPI());

	UI::FontMap fontMap(fontFace, FT_RENDER_MODE_NORMAL);


	while (!window.CloseRequested())
	{
		Window::PollInput();
		window.SwapBuffers();
	}

	return 0;
}
