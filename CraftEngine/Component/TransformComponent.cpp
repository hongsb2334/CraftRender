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

        //Todo: 트랜스폼이 안바뀌면 그냥 이전 값 쓸수 있다 (Dirty flag)
    }
}