//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Frame.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Frame::Frame()
		: mEventDispatcher(*this)
	{

	}


	Frame::Frame(std::unique_ptr<Node>&& root)
		: mRoot(std::move(root))
		, mEventDispatcher(*this)
	{}


	void Frame::Update(Core::Seconds deltaTime)
	{
		if (auto root = GetRoot()) root->Update(deltaTime);
	}


	void Frame::Render(Renderer& renderer)
	{
		if (auto root = GetRoot(); root && IsVisible())
		{
			root->Render(renderer);
		}
	}


	Core::Optional<Node*> Frame::GetRoot()
	{
		return mRoot ? Core::Optional(mRoot.get()) : Core::NullOpt;
	}


	Node* Frame::SetRoot(std::unique_ptr<Node> node)
	{
		mRoot = std::move(node);
		return mRoot.get();
	}


	bool Frame::Dispatch(const Window::Event& event)
	{
		if (mEventDispatcher.Dispatch(event)) return false;

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