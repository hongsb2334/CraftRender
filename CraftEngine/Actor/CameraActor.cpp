#include "CameraActor.h"
#include <Component/CameraComponent.h>


namespace Craft
{
    void CameraActor::Initialized()
    {
        Actor::Initialized();

        // 액터 초기화할 때 카메라 컴포넌트 추가 및 설정
        cameraComponent = AddComponent<CameraComponent>();
    }
    void CameraActor::SetProjection(float fieldOfView, float nearDistance, float farDistance)
    {
        cameraComponent->SetProjection(fieldOfView, nearDistance, farDistance);
    }
}