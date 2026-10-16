#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>

void TestActor::Initialized()
{
    Actor::Initialized();

    //스태틱 메시 컴포넌트 추가
    AddComponent<Craft::StaticMeshComponent>();


}

void TestActor::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    GetTransform().rotation.z += 60.0f * deltaTime;
}
