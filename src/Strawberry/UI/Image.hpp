#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Image.hpp"
#include "Node.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/Vulkan/Image.hpp"
// Strawberry Core
#include "Strawberry/Core/Types/ReflexivePointer.hpp"

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Renderer;


	class Image
		: public Node
	{
	public:
		Image(Graphics::Vulkan::Image& image);


		void Render(Renderer& renderer) override;


		void SetImage(Graphics::Vulkan::Image& image);
		Core::ReflexivePointer<Graphics::Vulkan::Image> GetImage() const;


	private:
		Core::ReflexivePointer<Graphics::Vulkan::Image> mImage;
	};
}