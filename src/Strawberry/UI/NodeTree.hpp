#pragma once


#include "Strawberry/Core/Math/Graph/Tree.hpp"
#include "Strawberry/Core/Math/Graph/TreeWalker.hpp"


namespace Strawberry::UI
{
	using NodeTreeType = Core::Math::OrderedTree<std::unique_ptr<Node>>;
	class NodeTree
		: public NodeTreeType
	{
		using NodeTreeType::NodeTreeType;
	};
}