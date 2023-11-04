#pragma once


// Strawberry Core
#include <Strawberry/Core/Types/ReflexivePointer.hpp>
#include <Strawberry/Core/Math/Vector.hpp>
// Standard Library
#include <memory>
#include <vector>


namespace Strawberry::UI
{
	class Node
		: public Core::EnableReflexivePointer<Node>
	{
	public:
		Node();
		Node(Node& node);


		[[nodiscard]] Core::Math::Vec2f GetPosition() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalPosition() const;

		[[nodiscard]] Core::Math::Vec2f GetSize() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalSize() const;

		[[nodiscard]] Core::Math::Vec2f GetScale() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalScale() const;


		[[nodiscard]] Core::ReflexivePointer<Node> GetChild(size_t index);


	private:
		Core::ReflexivePointer<Node> mParent;
		std::vector<std::unique_ptr<Node>> mChildren;

		Core::Math::Vec2f mLocalPosition = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalSize     = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalScale    = Core::Math::Vec2f(1.0f, 1.0f);
	};
}
