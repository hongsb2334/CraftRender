#pragma once
#include <Core/Core.h>
#include <Math/Vector2.h>
#include <cstdint>

namespace Craft
{
    //키 입력/마우스 입력을 처리하는 클래스
    //프레임 단위로 입력 변경 상태 처리

    class Input : public TGlobalAccessible<Input>
    {
        //키 입력 상태 구조체
        struct KeyState
        {
            // 현재 프레임에 처음 눌렸는지 여부
            bool isKeyDown = false;

            //현재 프레임에 눌림이 해제되었는지 여부
            bool isKeyUp = false;

            //현재 눌림 상태인지
            bool isKey = false;
        };

        //마우스 입력 상태 구조체
        struct MouseButtonState
        {
            // 현재 프레임에 처음 눌렸는지 여부
            bool isButtonDown = false;

            //현재 프레임에 눌림이 해제되었는지 여부
            bool isButtonUp = false;

            //현재 눌림 상태인지
            bool isButton = false;
        };
    public:
        Input() = default;
        ~Input() = default;

        //액터나 컴포넌트에서 입력 확인할 때 사용할 함수
        bool GetKeyDown(uint32_t keyCode) const;
        bool GetKeyUp(uint32_t keyCode) const;
        bool GetKey(uint32_t keyCode) const;

        //마우스 버튼 입력 확인할 때 사용할 함수
        bool GetMouseButtonDown(uint32_t button) const;
        bool GetMouseButtonUp(uint32_t button) const;
        bool GetMouseButton(uint32_t button) const;

        // 마우스 위치 반환 함수
        inline Vector2 GetMousePosition() const { return mousePosition; }

        //마우스 포인터의 이동량(Delta) 반환 함수 (X/Y)
        inline float GetMouseDeltaX() const
        {
            return mousePosition.x - mousePreviousPosition.x;
        }

        inline float GetMouseDeltaY() const
        {
            return mousePosition.y - mousePreviousPosition.y;
        }

        //Win32 메시지를 키 상태로 변환하는 함수
        void OnKeyDown(uint32_t keyCode);
        void OnKeyUp(uint32_t keyCode);

        //Win32 메시지를 마우스 버튼 입력 상태로 변환하는 함수
        void OnMouseButtonDown(uint32_t button);
        void OnMouseButtonUp(uint32_t button);

        //Win32 메시지를 마우스 위치 설정할 때 사용할 함수
        void SetMousePosition(int x, int y);

        //키입력 상태 전체를 초기화하는 함수
        //창이 포커스를 잃었을 때 처리를 위해
        void ResetAll();
        
        //프레임이 종료되면 리셋(초기화)
        void ResetInputs();

    private:
        //키코드가 유효한지 확인하는 함수
        bool IsValidKeyCode(uint32_t keyCode) const;

        //마우스 버튼 코드가 유효한지 확인하는 함수
        //0:왼쪽, 1: 오른쪽, 2: 가운데 버튼
        bool IsValidMouseButton(uint32_t button) const;

    private:
        //윈도우에 정의된 가상 키코드 개수 (256개)
        static const uint32_t keyStateCount = 256;

        //입력에 사용할 마우스 버튼 수
        static const uint32_t MouseButtonCount = 3;
        
        //키입력 상태를 관리할 배열
        KeyState keyStates[keyStateCount] = {};

        //키입력 상태를 관리할 배열
        MouseButtonState MouseButtonStates[MouseButtonCount] = {};

        //마우스 좌표 저장을 위한 변수
        Vector2 mousePosition = Vector2::Zero;
        Vector2 mousePreviousPosition = Vector2::Zero;

    };
}

