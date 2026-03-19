#include "Strawberry/UI/NodeTree.hpp"


namespace Strawberry::UI
{
	NodeTree::NodeTree()
		: NodeTreeType(std::make_unique<Node>())
	{}
}
