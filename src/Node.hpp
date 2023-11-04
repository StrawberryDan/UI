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


		Core::Math::Vec2i GetPosition() const;
		Core::Math::Vec2i GetLocalPosition() const;

		Core::Math::Vec2i GetSize() const;
		Core::Math::Vec2i GetLocalSize() const;

		Core::Math::Vec2f GetScale() const;
		Core::Math::Vec2f GetLocalScale() const;


		Core::ReflexivePointer<Node> GetChild(size_t index);


	private:
		Core::ReflexivePointer<Node> mParent;
		std::vector<std::unique_ptr<Node>> mChildren;

		Core::Math::Vec2i mLocalPosition;
		Core::Math::Vec2i mLocalSize;
		Core::Math::Vec2f mLocalScale;
	};
}
