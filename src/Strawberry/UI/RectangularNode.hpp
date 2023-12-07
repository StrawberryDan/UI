#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Node.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
    enum class RelativePosition
    {
        Above,
        Left,
        Right,
        Below,
    };


    class RectangularNode
        : public Node
    {
    public:
        void Update(Core::Seconds deltaTime) override;


        [[nodiscard]] Core::Math::Vec2f GetSize() const;
        [[nodiscard]] Core::Math::Vec2f GetLocalSize() const;


        void SetLocalSize(Core::Math::Vec2f size);


        virtual bool ContainsPoint(Core::Math::Vec2f screenPosition);


        void Center();
        void CenterHorizontally();
        void CenterVertically();
        void PositionRelative(RectangularNode& node, RelativePosition position, float padding = 0.0f);


    private:
        Core::Math::Vec2f mLocalSize = Core::Math::Vec2f(0.0f, 0.0f);
    };
}
