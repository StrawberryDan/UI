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


	void Frame::Update(float deltaTime)
	{
		for (auto& root : mRoots)
		{
			root->Update(deltaTime);
		}
	}


	void Frame::Render(Renderer& renderer)
	{
		for (auto& root : mRoots)
		{
			root->Render(renderer);
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


	void Frame::AddRoot(std::unique_ptr<Node> node)
	{
		mRoots.emplace_back(std::move(node));
	}
}