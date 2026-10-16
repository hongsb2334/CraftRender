#include "StaticMeshComponent.h"
#include <Graphics/Renderer.h>
#include <GameFramework/Actor.h>
#include <Math/Transform.h>

namespace Craft
{
    void StaticMeshComponent::Draw()
    {
        Component::Draw();

        //액터 얻기
        auto actor = GetOwner();

        if (!actor || !shader || !staticMesh)
        {
            return;
        }

        //트랜스폼 얻기
        const Transform& transform = actor->GetTransform();

        //렌더러에 제출
        Renderer::Get().Submit(staticMesh, shader, transform.GetWorldMatrix());
    }
}