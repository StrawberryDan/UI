#pragma once


// Strawberry Core
#include "Strawberry/Core/Types/ReflexivePointer.hpp"
#include "Strawberry/Core/Math/Vector.hpp"
// Standard Library
#include <memory>
#include <vector>


namespace Strawberry::UI
{
	class Renderer;


	class Node
		: public Core::EnableReflexivePointer<Node>
	{
	public:
		Node();


		virtual void Update() {}


		virtual void Render(Renderer& renderer) = 0;


		[[nodiscard]] Core::Math::Vec2f GetPosition() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalPosition() const;

		[[nodiscard]] Core::Math::Vec2f GetSize() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalSize() const;

		[[nodiscard]] Core::Math::Vec2f GetScale() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalScale() const;


		void SetLocalPosition(Core::Math::Vec2f position);
		void SetLocalSize(Core::Math::Vec2f size);
		void SetLocalScale(Core::Math::Vec2f scale);


		[[nodiscard]] Core::ReflexivePointer<Node> GetParent() const;


		[[nodiscard]] size_t GetChildCount() const;

		[[nodiscard]] Core::ReflexivePointer<Node> GetChild(size_t index);

		Core::ReflexivePointer<Node> AppendChild(std::unique_ptr<Node> node);
		Core::ReflexivePointer<Node> PrependChild(std::unique_ptr<Node> node);
		Core::ReflexivePointer<Node> InsertChild(size_t index, std::unique_ptr<Node> node);


	private:
		Core::ReflexivePointer<Node> mParent;
		std::vector<std::unique_ptr<Node>> mChildren;

		Core::Math::Vec2f mLocalPosition = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalSize     = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalScale    = Core::Math::Vec2f(1.0f, 1.0f);
	};
}
