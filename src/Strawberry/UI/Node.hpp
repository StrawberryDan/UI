#pragma once
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::UI
{
	class NodeRenderer;

	/// Class representing a UI node. A Node possesses a position and an extent.
	class Node
	{
	public:
		using Position = Core::Math::Vec2f;
		using Extent   = Core::Math::Vec2f;
		using Layer    = int32_t;
		static constexpr Layer NO_LAYER = std::numeric_limits<Layer>::lowest();


		Node() = default;
		Node(Position position, Extent extent);
		virtual ~Node() = default;


		Position GetPosition()            const noexcept { return mPosition; }
		Position SetPosition(Position position) noexcept { return mPosition = position; }


		Extent GetExtent()        const noexcept { return mExtent; }
		Extent SetExtent(Extent extent) noexcept { return mExtent = extent; }


		virtual void Render(NodeRenderer& nodeRenderer) const = 0;


	private:
		Position mPosition;
		Extent   mExtent;
		Layer    mLayer = NO_LAYER;
	};
}
