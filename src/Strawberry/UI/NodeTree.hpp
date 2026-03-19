#pragma once


#include "Node.hpp"
#include "Strawberry/Core/Math/Graph/Tree.hpp"


namespace Strawberry::UI
{
	using NodeTreeType = Core::Math::OrderedTree<std::unique_ptr<Node>>;
	class NodeTree
		: public NodeTreeType
	{
	public:
		NodeTree();
		// using NodeTreeType::NodeTreeType;


		using NodeTreeType::AddNode;
		template <typename N>
		Config::NodeID AddNode(Config::NodeID parent, N&& n)
		{
			return AddNode(parent, std::make_unique<N>(std::forward<N>(n)));
		}
	};
}