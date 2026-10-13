#include "TestActor.h"
#include <Component/TransformComponent.h>
void TestActor::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    GetTransform().rotation.z += 60.0f * deltaTime;
}
