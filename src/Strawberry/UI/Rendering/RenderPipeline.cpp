//======================================================================================================================
//		Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/UI/Rendering/RenderPipeline.hpp"
// Strawberry Vulkan
#include "Strawberry/Vulkan/RenderPass.hpp"


//======================================================================================================================
//		Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	RenderPipeline::Layout::Layout(const Vulkan::Device& device)
		: Vulkan::PipelineLayout([&]()
		{
			return Vulkan::PipelineLayout::Builder(device)
				.Build();
		}()){}
	RenderPipeline::RenderPipeline(const Vulkan::RenderPass& renderPass)
		: RenderPipeline(Layout(*renderPass.GetDevice()), renderPass)
	{}


	RenderPipeline::RenderPipeline(Layout&& layout, const Vulkan::RenderPass& renderPass)
		: Vulkan::GraphicsPipeline([&]()
		{
			return GraphicsPipeline::Builder(layout, renderPass)
					.Build();
		}())
		, mRenderPass(renderPass)
		, mLayout(std::move(layout)) {}
}
