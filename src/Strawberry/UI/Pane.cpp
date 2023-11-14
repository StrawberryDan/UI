#include "Pane.hpp"
#include "Rendering/Renderer.hpp"


namespace Strawberry::UI
{

	void Pane::Render(Renderer& renderer)
	{
		if (GetVisible())
		{
			renderer.Render(*this);

			for (int i = 0; i < GetChildCount(); i++)
				GetChild(i)->Render(renderer);
		}
	}


	const Core::Math::Vec4f& Pane::GetFillColor() const
	{
		return mFillColor;
	}


	void Pane::SetFillColor(const Core::Math::Vec4f& fillColor)
	{
		mFillColor = fillColor;
	}
}
