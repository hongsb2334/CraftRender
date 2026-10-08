#include "TransformComponent.h"
#include <Graphics/Renderer.h>

namespace Craft
{
    void TransformComponent::Draw()
    {
        Component::Draw();

        //트랜스폼 업데이트
        transform.Update();

        //Temp: 렌더러에  월드 행렬 제출, 얘 때문에 액터가 태어날때 순서상 TransformComponent가 가장 먼저 생성된다.
        Renderer::Get().Submit(transform.GetWorldMatrix());
    }
}