#pragma once


// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
// Standard Library
#include <memory>
#include <vector>


namespace Strawberry::UI
{
	class Renderer;


	class Node
		: public std::enable_shared_from_this<Node>
	{
	public:
		Node();


		virtual ~Node() = default;


		virtual void Update() {}


		virtual void Render(Renderer& renderer) {}


		[[nodiscard]] Core::Math::Vec2f GetPosition() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalPosition() const;

		[[nodiscard]] Core::Math::Vec2f GetSize() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalSize() const;

		[[nodiscard]] Core::Math::Vec2f GetScale() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalScale() const;


		void SetLocalPosition(Core::Math::Vec2f position);
		void SetLocalSize(Core::Math::Vec2f size);
		void SetLocalScale(Core::Math::Vec2f scale);


		[[nodiscard]] std::shared_ptr<Node> GetParent() const;


		[[nodiscard]] size_t GetChildCount() const;

		[[nodiscard]] std::shared_ptr<Node> GetChild(size_t index);


		template <std::derived_from<Node> T, typename... Args>
		std::shared_ptr<T> AppendChild(Args&&... args)
		{
			auto node = std::make_shared<T>(std::forward<Args&&>(args)...);
			AppendChild(std::static_pointer_cast<Node>(node));
			return node;
		}

		std::shared_ptr<Node> AppendChild(std::shared_ptr<Node> node);
		std::shared_ptr<Node> PrependChild(std::shared_ptr<Node> node);
		std::shared_ptr<Node> InsertChild(size_t index, std::shared_ptr<Node> node);


	private:
		std::weak_ptr<Node> mParent;
		std::vector<std::shared_ptr<Node>> mChildren;

		Core::Math::Vec2f mLocalPosition = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalSize     = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalScale    = Core::Math::Vec2f(1.0f, 1.0f);
	};
}
