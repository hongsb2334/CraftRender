#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>
#include <Core/Input.h>
#include <iostream>

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

    // W키 입력처리
    Craft::Input& input = Craft::Input::Get();

    if (input.GetKeyDown('W'))
    {
        std::cout << "W Key down\n";
    }

    if (input.GetKeyUp('W'))
    {
        std::cout << "W Key up\n";
    }

    if (input.GetKey('W'))
    {
        std::cout << "W Key hold\n";
    }
}
