#include "Strawberry/Core/IO/Logging.hpp"
#include "Strawberry/UI/ColoredNode.hpp"
#include "Strawberry/UI/Node.hpp"
#include "Strawberry/UI/Rendering/ColoredNodeRenderer.hpp"
#include "Strawberry/UI/Rendering/Renderer.hpp"
#include "Strawberry/Vulkan/Instance.hpp"
#include "Strawberry/Vulkan/Device.hpp"
#include "Strawberry/Vulkan/Swapchain.hpp"
#include "Strawberry/Vulkan/Surface.hpp"
#include "Strawberry/Window/Window.hpp"
#include "Strawberry/UI/Rendering/Text/FontFace.hpp"
#include "Strawberry/UI/Rendering/Text/FontMap.hpp"
#include "Strawberry/Vulkan/RenderPass.hpp"
#include "Strawberry/Vulkan/Memory/NaiveAllocator.hpp"
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

	Vulkan::RenderPass renderPass = Vulkan::RenderPass::Builder(device)
		.WithColorAttachment(
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			VK_FORMAT_R8G8B8A8_SRGB,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_IMAGE_LAYOUT_GENERAL)
		.WithSubpass(
			Vulkan::SubpassDescription()
				.WithColorAttachment(0, VK_IMAGE_LAYOUT_GENERAL))
		.Build();

	Vulkan::NaiveAllocator frameBufferAllocator(
	device, physicalDevice.SearchMemoryTypes(Vulkan::MemoryTypeCriteria::DeviceLocal()).front().index);

	Vulkan::Framebuffer framebuffer(renderPass, frameBufferAllocator, window.GetSize().AsType<unsigned>());


	UI::FontFace fontFace = UI::FontFace::FromFile("data/italianno.ttf").Unwrap();
	fontFace.SetSizePoints(40, Window::GetMonitorInfo()[0].GetDPI());

	UI::FontMap fontMap(fontFace, FT_RENDER_MODE_NORMAL);


	UI::Renderer renderer(framebuffer, 0);;
	UI::ColoredNode root;
	root.SetExtent({400, 600});
	root.SetColor({1.0f, 1.0f, 0.0f, 1.0f});

	UI::ColoredNode root2;
	root2.SetPosition({100.0f, 100.0f});
	root2.SetExtent({100, 100});
	root2.SetColor({0.0f, 0.0f, 1.0f, 1.0f});


	Vulkan::CommandPool commandPool(queue);


	while (!window.CloseRequested())
	{
		Window::PollInput();

		Vulkan::CommandBuffer commandBuffer(commandPool);
		commandBuffer.Begin(true);
		commandBuffer.PipelineBarrier(
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			0,
			{
				Vulkan::ImageMemoryBarrier(
					framebuffer.GetAttachment(0),
					VK_IMAGE_ASPECT_COLOR_BIT).FromLayout(VK_IMAGE_LAYOUT_UNDEFINED)
				.ToLayout(VK_IMAGE_LAYOUT_GENERAL)
										   	});;
		commandBuffer.BeginRenderPass(renderPass, framebuffer);
		renderer.Submit(root);
		renderer.Submit(root2);
		renderer.Render(commandBuffer);
		commandBuffer.EndRenderPass();
		commandBuffer.BlitToSwapchain(swapchain, framebuffer);
		commandBuffer.End();

		queue.Submit(commandBuffer);
		swapchain.Present();

		queue.WaitUntilIdle();
	}

	return 0;
}
