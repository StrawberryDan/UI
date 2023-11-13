//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Image.hpp"
#include "Rendering/Renderer.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Image::Image(Graphics::Vulkan::Image& image)
		: mImage(image.GetReflexivePointer())
	{}


	void Image::Render(Renderer& renderer)
	{
		renderer.Render(*this);
	}


	void Image::SetImage(Graphics::Vulkan::Image& image)
	{
		mImage = image.GetReflexivePointer();
	}


	Core::ReflexivePointer<Graphics::Vulkan::Image> Image::GetImage() const
	{
		return mImage;
	}
}