#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "RectangularNode.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/Vulkan/Image.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Button
		: public RectangularNode
	{
	public:
		using Callback = std::function<void()>;


		Button(Graphics::Vulkan::Image& image);
		Button(Graphics::Vulkan::Image& image, Callback callback);


		void Render(Renderer& renderer) override;


		Core::ReflexivePointer<Graphics::Vulkan::Image> GetImage() const { return mImage; }


		void SetCallback(Callback callback);


	private:
		Core::ReflexivePointer<Graphics::Vulkan::Image> mImage;
		Callback mCallback;
	};
}