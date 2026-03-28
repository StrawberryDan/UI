#pragma once
#include "Strawberry/Core/Math/Vector.hpp"
#include "Strawberry/UI/Node.hpp"

namespace Strawberry::UI
{
	struct RenderContext
	{
		Core::Math::Vec2f position;


		RenderContext Apply(const Node& node) const
		{
			RenderContext result;

			result.position = this->position + node.GetPosition();

			return result;
		}
	};
}