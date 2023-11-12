#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Strawberry/Graphics/Vulkan/Pipeline.hpp"
#include "Strawberry/Graphics/Vulkan/DescriptorSet.hpp"
#include "Strawberry/Graphics/Vulkan/Buffer.hpp"
#include "Strawberry/Graphics/Vulkan/Queue.hpp"
#include "Strawberry/Graphics/Vulkan/RenderPass.hpp"
#include "Strawberry/Graphics/Vulkan/Framebuffer.hpp"
// Strawberry Core
#include "Strawberry/Core/Types/Optional.hpp"
#include "Strawberry/Graphics/Renderer.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Pane;


	class RectangleRenderer
		: public Graphics::Renderer
	{
	public:
		RectangleRenderer(Graphics::Vulkan::Queue& queue, Core::Math::Vec2u renderSize);


		void Render(const Pane& pane);


	protected:
		static Graphics::Vulkan::RenderPass CreateRenderPass(const Graphics::Vulkan::Device& device);
		static Graphics::Vulkan::Pipeline CreatePipeline(const Graphics::Vulkan::RenderPass& renderPass,
														 Core::Math::Vec2u renderSize);


	private:
		Graphics::Vulkan::Pipeline mPipeline;
		Graphics::Vulkan::DescriptorSet mVertexDescriptorSet;
		Graphics::Vulkan::Buffer mVertexUniformBuffer;
	};
}