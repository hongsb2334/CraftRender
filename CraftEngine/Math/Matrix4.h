#pragma once
#include "Vector3.h"

namespace Craft
{
    //변환 함수를 제공하는 4x4 행렬
    class Matrix4
    {
    public:
        Matrix4();
        ~Matrix4() = default;

        Matrix4(const Matrix4& other);
        Matrix4& operator=(const Matrix4& other);
        
        //행렬 곱 연산자 오버로딩
        Matrix4 operator*(const Matrix4& other) const;
        Matrix4& operator*=(const Matrix4& other);

        //전치 함수(행과 열을 바꾸는 함수)
        static Matrix4 Transpose(const Matrix4& matrix);

        //회전행렬의 역행렬 함수 - 전치동작을 역행렬 동작으로 구현
        //행렬의 역행렬 함수는 안구함
        //주의 - 기저 축이 서로 직교일때만 처리
        //직교행렬의 역행렬은 전치행렬이다.
        //원래 순수 역행렬은 구하기 복잡
        //모든 상황에서 해가 있지 않아서 이걸 먼저 처리해줘야함.
        //노멀맵에서 순수 역행렬을 구해야 하는 상황이 있기는 함
        //그냥 스케일 구하는 거에서는 순수 역행렬 구하지 않아도 됨
        static Matrix4 InverseRotation(const Matrix4& matrix);

        //변환 행렬 생성 함수
        

        //이동 변환 행렬
        static Matrix4 Translation(float x, float y, float z);
        static Matrix4 Translation(const Vector3& translation);




        //크기 변환 행렬
        static Matrix4 Scale(float x, float y, float z);
        static Matrix4 Scale(const Vector3& scale);
        static Matrix4 Scale(float scale);



        //내부에서 관리하는 배열의 원시 포인터 반환 함수
        const float* Data() const { return elements; }

        
        //벡터와 행렬 곱
        //벡터와 행렬 곱을 처리할 때 행벡터로 사용해서 처리
        //행벡터를 사용하는 경우 벡터가 왼쪽에 배치되도록 처리 (벡터 * 행렬)
        friend Vector3 operator*(const Vector3& vector, const Matrix4& matrix);


    public:
        //단위 행렬 상수
        //대각 성분이 1이고 나머지 0인 행렬
        //행렬의 곱 했을 때 원래 행렬 반환
        //행렬 곱의 항등원
        static const Matrix4 Identity;

    private:
        // 4x4 행렬 변수 선언
        //공용체 -> 여러 변수가 같은 메모리 공간을 함께 사용
        union 
        {
            struct
            {
                float m00, m01, m02, m03;   //1행
                float m10, m11, m12, m13;   //2행
                float m20, m21, m22, m23;   //3행
                float m30, m31, m32, m33;   //4행
            };

            //배열을 한번에 선언, 4x4행렬이라는 의도를 보여주기 위해 4*4라고 표현
            float elements[4 * 4];
        };
    };
}


