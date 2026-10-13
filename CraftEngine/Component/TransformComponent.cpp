#include "TransformComponent.h"

namespace Craft
{
    TransformComponent::TransformComponent()
    {
    }

    void TransformComponent::Draw()
    {
        Component::Draw();

        //트랜스폼 업데이트
        transform.Update();

    }
}