#include "Matrix4.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include "MathDefines.h"
#include <cassert>
#include <DirectXMath.h>

using namespace DirectX;
int XMMatrixLookAtLH();

namespace Craft
{
    const Matrix4 Matrix4::Identity = Matrix4();

    Matrix4::Matrix4()
    {
        memset(elements, 0, sizeof(elements));

        //대각 성분만 1로 설정
        m00 = 1.0f;
        m11 = 1.0f;
        m22 = 1.0f;
        m33 = 1.0f;

    }
    Matrix4::Matrix4(const Matrix4& other)
    {
        //메모리 통복사
        memcpy(elements, other.elements, sizeof(elements));
    }
    Matrix4& Matrix4::operator=(const Matrix4& other)
    {
        memcpy(elements, other.elements, sizeof(elements));
        return *this;
    }
    Matrix4 Matrix4::operator*(const Matrix4& other) const
    {
        //결과를 반환할 행렬 변수 선언
        Matrix4 m;

        //1행 성분 계산, 벡터 내적 연산과 같은 형태로 계산
        m.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
        m.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
        m.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
        m.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;
        
        //2행
        m.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
        m.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
        m.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
        m.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;

        //3행
        m.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
        m.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
        m.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
        m.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;

        //4행
        m.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;
        m.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
        m.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
        m.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

        return m;
    }
    Matrix4& Matrix4::operator*=(const Matrix4& other)
    {
        *this = *this * other;
        return *this;
    }
    Matrix4 Matrix4::Transpose(const Matrix4& matrix)
    {
        //반환할 행렬 선언
        Matrix4 m = matrix;
        //대각 성분을 기준으로 행과 열 바꿈
        std::swap(m.m01, m.m10);
        std::swap(m.m02, m.m20);
        std::swap(m.m03, m.m30);
        std::swap(m.m21, m.m12);
        std::swap(m.m31, m.m13);
        std::swap(m.m32, m.m23);

        return m;
    }
    Matrix4 Matrix4::InverseRotation(const Matrix4& matrix)
    {
        return Transpose(matrix);
    }

    Matrix4 Matrix4::Translation(float x, float y, float z)
    {
        Matrix4 m;
        
        m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
        m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = 0.0f;
        m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = 0.0f;
        m.m30 = x; m.m31 = y; m.m32 = z; m.m33 = 1.0f;


        return m;
    }

    Matrix4 Matrix4::Translation(const Vector3& translation)
    {
        return Translation(translation.x, translation.y, translation.z);
    }

    Matrix4 Matrix4::Scale(float x, float y, float z)
    {
        //
        Matrix4 m;

        m.m00 = x; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
        m.m10 = 0.0f; m.m11 = y; m.m12 = 0.0f; m.m13 = 0.0f;
        m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = z; m.m23 = 0.0f;
        m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;



        return m;
    }

    Matrix4 Matrix4::Scale(const Vector3& scale)
    {
        return Scale(scale.x, scale.y, scale.z);
    }

    Matrix4 Matrix4::Scale(float scale)
    {
        return Scale(scale, scale, scale);
    }

    Matrix4 Matrix4::RotationX(float angle)
    {
        //코사인/사인 값 구하기
        float cosAngle = std::cos(angle * DegreesToRadians);
        float sinAngle = std::sin(angle * DegreesToRadians);


        // [c s]
        // [-s c]

        Matrix4 m;

        m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
        m.m10 = 0.0f; m.m11 = cosAngle; m.m12 = sinAngle; m.m13 = 0.0f;
        m.m20 = 0.0f; m.m21 = -sinAngle; m.m22 = cosAngle; m.m23 = 0.0f;
        m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;



        return m;
    }

    Matrix4 Matrix4::RotationY(float angle)
    {
        //코사인/사인 값 구하기
        //y축은 sin 값 뒤집음
        float cosAngle = std::cos(angle * DegreesToRadians);
        float sinAngle = std::sin(angle * DegreesToRadians);


        // [c s]
        // [-s c]

        Matrix4 m;

        m.m00 = cosAngle; m.m01 = 0.0f; m.m02 = -sinAngle; m.m03 = 0.0f;
        m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = 0.0f;
        m.m20 = sinAngle; m.m21 = 0.0f; m.m22 = cosAngle; m.m23 = 0.0f;
        m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;



        return m;
    }

    Matrix4 Matrix4::RotationZ(float angle)
    {
        //코사인/사인 값 구하기
        float cosAngle = std::cos(angle * DegreesToRadians);
        float sinAngle = std::sin(angle * DegreesToRadians);


        // [c s]
        // [-s c]

        Matrix4 m;

        m.m00 = cosAngle; m.m01 = sinAngle; m.m02 = 0.0f; m.m03 = 0.0f;
        m.m10 = -sinAngle; m.m11 = cosAngle; m.m12 = 0.0f; m.m13 = 0.0f;
        m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = 0.0f;
        m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;



        return m;
    }

    Matrix4 Matrix4::Rotation(float x, float y, float z)
    {
        //무조건은 아니지만 일반적으로 x->y->z 순으로 회전 적용


        return RotationX(x) *RotationY(y) * RotationZ(z);
    }

    Matrix4 Matrix4::Rotation(const Vector3& rotation)
    {
        
        return Rotation(rotation.x, rotation.y, rotation.z);
    }

    Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& upVector)
    {
        //회전 성분 구하기
        //-> 정규 직교 벡터들 구하기 (Forward, right, up)

        //카메라의 앞방향 구하기
        Vector3 forward = target - eye;

        //카메라 위치와 바라보는 위치가 같으면 방향 못구해서 안됨
        assert(forward.Length() > KindaSmallNumber);

        //정규화(길이 1로 만들기)
        forward.Normalized();

        //앞방향과 직교(90도)하는 오른쪽 방향
        Vector3 right = upVector.Normalized().Cross(forward);

        //upvector와 forward가 겹치면 안됨
        assert(right.Length() > KindaSmallNumber);
        right = right.Normalized();

        //위쪽 방향 구하기
        //
        Vector3 up = forward.Cross(right).Normalized();

        //위치 성분에 들어갈 값 계산
        float x = -eye.Dot(right);
        float y = -eye.Dot(up);
        float z = -eye.Dot(forward);

        Matrix4 m;
        
        m.m00 = right.x; m.m01 = up.x; m.m02 = forward.x; m.m03 = 0.0f;
        m.m10 = right.y; m.m11 = up.y; m.m12 = forward.y; m.m13 = 0.0f;
        m.m20 = right.z; m.m21 = up.z; m.m22 = forward.z; m.m23 = 0.0f;
        m.m30 = x;       m.m31 = y;    m.m32 = z;         m.m33 = 1.0f;

        return m;
    }

    Matrix4 Matrix4::Perspective(float fieldOfView, float width, float height, float nearDistance, float farDistance)
    {
        //값 확인
        assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
        assert(width > 0.0f && height > 0.0f);
        assert(nearDistance > 0.0f && farDistance > nearDistance);



        //화면비 구하기 (Aspect Ratio)
        float aspect = width / height;

        //초점 거리 (focal length)
        float d = 1.0f / std::tan(fieldOfView * 0.5f * DegreesToRadians);

        //Y의 스케일 계수
        float sy = d;

        //X의 스케일 계수
        float sx = sy / aspect;

        //A와 B
        float a = farDistance / (farDistance - nearDistance);
        float b = -nearDistance * a;


        Matrix4 m;
        m.m00 = sx;     m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
        m.m10 = 0.0f;	m.m11 = sy;	    m.m12 = 0.0f;	m.m13 = 0.0f;
        m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = a;	    m.m23 = 1.0f;
        m.m30 = 0.0f;	m.m31 = 0.0f;	m.m32 = b;	    m.m33 = 0.0f;

        return m;
    }

    Matrix4 Matrix4::InverseTranslationRotation(const Vector3& position, const Vector3& rotation)
    {
        //순방향 변환 -> S X R X T
        //역방향 반환 -> T^-1 X R^-1 X S^-1
        return Translation(position * -1.0f) * InverseRotation(Rotation(rotation));
    }

    Vector3 operator*(const Vector3& vector, const Matrix4& matrix)
    {
        //행벡터로 취급해서 계산, vector는 1x3 이고, 행렬은 4x4라 안맞아서 vector를 1x4 행렬 형태로 취급
        //4번째 열의 성분은 1로 가정
        //4번째 열의 성분이 1이면 점(위치) / 0이면 방향
        Vector3 v;
        v.x = vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20 + matrix.m30;
        v.y = vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21 + matrix.m31;
        v.z = vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22 + matrix.m32;
        
        

        return v;
    }
}