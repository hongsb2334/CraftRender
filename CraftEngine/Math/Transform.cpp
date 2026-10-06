#include "Transform.h"

namespace Craft
{
    Transform::Transform()
    {
        Update();
    }
    void Transform::Update()
    {
        //월드 행렬 업데이트
        //변환 곱셈 순서 중요(SRT 순서대로)
        //그래픽스 면접 문제에 자주 출제
        //로컬 기준의 좌표/회전/스케일을 공통된 기준인 월드 기준으로 바꾸는 것, 매 프레임마다 업데이트
        worldMatrix = Matrix4::Scale(scale) * Matrix4::Rotation(rotation) * Matrix4::Translation(position);
    }
    Vector3 Transform::Right() const
    {
        //Matrix4::Rotation(rotation)에서도 정보를 이미 제공하고 있어서 두 가지 방법 다 숙지 필요
        return Vector3::Right * Matrix4::Rotation(rotation);
    }
    Vector3 Transform::Up() const
    {
        return Vector3::Up * Matrix4::Rotation(rotation);
    }
    Vector3 Transform::Forward() const
    {
        return Vector3::Forward * Matrix4::Rotation(rotation);
    }
}