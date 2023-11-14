//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Frame.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Frame::Frame() = default;


	Frame::Frame(std::unique_ptr<Node>&& root)
	{
		mRoots.emplace_back(std::move(root));
	}


	void Frame::Update(Core::Seconds deltaTime)
	{
		for (auto& root : mRoots)
		{
			root->Update(deltaTime);
		}
	}


	void Frame::Render(Renderer& renderer)
	{
		if (IsVisible())
		{
			for (auto& root: mRoots)
			{
				root->Render(renderer);
			}
		}
	}


	uint32_t Frame::GetRootCount() const
	{
		return mRoots.size();
	}


	Node& Frame::GetRoot(uint32_t index)
	{
		return *mRoots[index];
	}


	Node* Frame::AddRoot(std::unique_ptr<Node> node)
	{
		mEventDispatchers.emplace_back(*this, *node);
		mRoots.emplace_back(std::move(node));
		return mRoots.back().get();
	}


	bool Frame::Dispatch(const Graphics::Window::Event& event)
	{
		for (int i = 0; i < GetRootCount(); i++)
		{
			if (!mEventDispatchers[i].Dispatch(event)) return false;
		}

		return true;
	}


	bool Frame::IsVisible() const
	{
		return mVisible;
	}


	void Frame::SetVisible(bool visible)
	{
		mVisible = visible;
	}


	Core::ReflexivePointer<Node> Frame::GetFocus()
	{
		return mFocus;
	}


	void Frame::SetFocus(Node& node)
	{
		mFocus = node.GetReflexivePointer();
	}
}