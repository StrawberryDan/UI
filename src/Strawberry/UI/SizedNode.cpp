//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SizedNode.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Core::Math::Vec2f SizedNode::GetSize() const
	{
		return GetScale() * GetLocalSize();
	}


	Core::Math::Vec2f SizedNode::GetLocalSize() const
	{
		return mLocalSize;
	}


	void SizedNode::SetLocalSize(Core::Math::Vec2f size)
	{
		mLocalSize = size;
	}


	bool SizedNode::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		auto relative = screenPosition - GetPosition();
		const Core::Math::Vec2f size = GetSize();
		return relative[0] >= 0.0f && relative[0] <= size[0] && relative[1] >= 0.0f && relative[1] <= size[1];
	}
}