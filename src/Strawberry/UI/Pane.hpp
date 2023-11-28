#pragma once


#include "RectangularNode.hpp"


namespace Strawberry::UI
{
	class Pane
		: public RectangularNode
	{
	public:
		void Render(Renderer& renderer) override;


		const Core::Math::Vec4f& GetFillColor() const;
		void SetFillColor(const Core::Math::Vec4f& fillColor);


	private:
		Core::Math::Vec4f mFillColor = Core::Math::Vec4f(0.5f, 0.5f, 0.5f, 1.0f);
	};
}
