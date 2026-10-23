
#pragma once
#include <Component/Component.h>

namespace Craft
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent();
        ~CameraComponent();

        virtual void Draw() override;

        //투영 관련 속성 설정 함수
        void SetProjection(float fieldOfView, float nearDistance = 0.1f, float farDistance = 1000.0f);

    private:
        //시야각 (단위 : 각도)
        float fieldOfView = 60.0f;


        //근평면까지의 거리
        float nearDistance = 0.1f;
        //원평면까지의 거리
        float farDistance = 1000.0f;



    };
}


