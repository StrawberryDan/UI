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
	{
		SetLocalSize(image.GetSize().AsType<float>().AsSize<2>());
	}


	void Image::Render(Renderer& renderer)
	{
		renderer.Render(*this);

		for (int i = 0; i < GetChildCount(); i++)
			GetChild(i)->Render(renderer);
	}


	void Image::SetImage(Graphics::Vulkan::Image& image)
	{
		mImage = image.GetReflexivePointer();
	}


	Core::ReflexivePointer<Graphics::Vulkan::Image> Image::GetImage() const
	{
		return mImage;
	}


	bool Image::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		if (!Node::ContainsPoint(screenPosition)) return false;

		Core::AssertEQ(mImage->GetFormat(), VK_FORMAT_R8G8B8A8_SRGB);
		Core::Math::Vec2u pixel = (screenPosition - GetPosition())
			 .Map([](float x) { return std::round(x); }).AsType<unsigned int>();
		auto pixelBytes = mImage->ReadPixel(pixel);

		return pixelBytes[0] > 0 && pixelBytes[1] > 0 && pixelBytes[2] > 0 && pixelBytes[3] > 0;
	}
}