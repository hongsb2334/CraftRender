#include "CameraComponent.h"
#include <Graphics/Renderer.h>
#include <Core/Engine.h>
#include <Math/Transform.h>

namespace Craft
{
    CameraComponent::CameraComponent()
    {
    }

    CameraComponent::~CameraComponent()
    {
    }

    void CameraComponent::Draw()
    {
        Component::Draw();

        //뷰변환 행렬 생성 및 전달
        auto actor = GetOwner();

        //트랜스폼 얻기
        const Transform& transform = actor->GetTransform();

        //카메라 변환 행렬 생성
        Matrix4 view = Matrix4::InverseTranslationRotation(transform.position, transform.rotation);

        //투영 변환 행렬 생성 및 전달
        float width = static_cast<float>(Engine::Get().GetClientWidth());
        float height = static_cast<float>(Engine::Get().GetClientHeight());

        if (width == 0.0f || height == 0.0f)
        {
            return;
        }

        Matrix4 projection = Matrix4::Perspective(fieldOfView, width, height, nearDistance, farDistance);

        Renderer::Get().SubmitCamera(view, projection);

    }
    void CameraComponent::SetProjection(float fieldOfView, float nearDistance, float farDistance)
    {
        this->fieldOfView = fieldOfView;
        this->nearDistance = nearDistance;
        this->farDistance = farDistance;
    }
}

