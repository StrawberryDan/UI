#include "Pane.hpp"
#include "Rendering/Renderer.hpp"


namespace Strawberry::UI
{

	void Pane::Render(Renderer& renderer)
	{
		renderer.Render(*this);

		for (int i = 0; i < GetChildCount(); i++)
		{
			GetChild(i)->Render(renderer);
		}
	}
}