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
	RenderPipeline::Layout::Layout(Vulkan::Device& device)
		: PipelineLayout([&]()
		{
			return Builder(device)
				.Build();
		}()){}
	RenderPipeline::RenderPipeline(Vulkan::RenderPass& renderPass)
		: RenderPipeline(Layout(*renderPass.GetDevice()), renderPass)
	{}


	RenderPipeline::RenderPipeline(Layout&& layout, Vulkan::RenderPass& renderPass)
		: GraphicsPipeline([&]()
		{
			return GraphicsPipeline::Builder(layout, renderPass)
					.Build();
		}())
		, mRenderPass(renderPass)
		, mLayout(std::move(layout)) {}
}
