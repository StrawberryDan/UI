#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/Vulkan/GraphicsPipeline.hpp"
#include "Strawberry/Vulkan/PipelineLayout.hpp"
#include "Strawberry/Vulkan/Device.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	//==============================================================================
	//		RenderPipeline class
	//
	//		Pipeline for the main load of UI rendering tasks.
	//==============================================================================
	class RenderPipeline
			: public Vulkan::GraphicsPipeline
	{
	public:
		//==============================================================================
		//		RenderPipeline::Layout class
		//
		//		Layout for the RenderPipeline class.
		//==============================================================================
		class Layout
				: public Vulkan::PipelineLayout
		{
		public:
			explicit Layout(const Vulkan::Device& device);
		};


		//==============================================================================
		//		Constructors
		//==============================================================================
	public:
		explicit RenderPipeline(const Vulkan::RenderPass& renderPass);

	private:
		RenderPipeline(Layout&& layout, const Vulkan::RenderPass& renderPass);


		//==============================================================================
		//		Methods
		//==============================================================================
	public:
	private:
		Core::ReflexivePointer<Vulkan::RenderPass> mRenderPass;
		Layout                                     mLayout;
	};
}
