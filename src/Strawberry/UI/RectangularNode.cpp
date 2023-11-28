//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "RectangularNode.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Core::Math::Vec2f RectangularNode::GetSize() const
	{
		return GetScale() * GetLocalSize();
	}


	Core::Math::Vec2f RectangularNode::GetLocalSize() const
	{
		return mLocalSize;
	}


	void RectangularNode::SetLocalSize(Core::Math::Vec2f size)
	{
		mLocalSize = size;
	}


	bool RectangularNode::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		auto relative = screenPosition - GetPosition();
		const Core::Math::Vec2f size = GetSize();
		return relative[0] >= 0.0f && relative[0] <= size[0] && relative[1] >= 0.0f && relative[1] <= size[1];
	}


	void RectangularNode::CenterInParent()
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());
		SetLocalPosition(GetParent<RectangularNode>()->GetSize() / 2.0f - GetSize() / 2.0f);
	}
}
