#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Image.hpp"
#include "RectangularNode.hpp"
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
		: public RectangularNode
	{
	public:
		Image(Graphics::Vulkan::Image& image);


		void Render(Renderer& renderer) override;


		void SetImage(Graphics::Vulkan::Image& image);
		Core::ReflexivePointer<Graphics::Vulkan::Image> GetImage() const;


		bool ContainsPoint(Core::Math::Vec2f screenPosition) override;


	private:
		Core::ReflexivePointer<Graphics::Vulkan::Image> mImage;
	};
}