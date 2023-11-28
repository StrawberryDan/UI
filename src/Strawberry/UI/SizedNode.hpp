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
    class SizedNode
        : public Node
    {
    public:
        [[nodiscard]] Core::Math::Vec2f GetSize() const;
        [[nodiscard]] Core::Math::Vec2f GetLocalSize() const;


        void SetLocalSize(Core::Math::Vec2f size);


        virtual bool ContainsPoint(Core::Math::Vec2f screenPosition);


    private:
        Core::Math::Vec2f mLocalSize = Core::Math::Vec2f(0.0f, 0.0f);
    };
}
