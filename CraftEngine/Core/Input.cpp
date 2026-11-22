#include "Input.h"


namespace Craft
{
    bool Input::GetKeyDown(uint32_t keyCode) const
    {
        return IsValidKeyCode(keyCode) && keyStates[keyCode].isKeyDown ;
    }
    bool Input::GetKeyUp(uint32_t keyCode) const
    {
        return IsValidKeyCode(keyCode) && keyStates[keyCode].isKeyUp;
    }
    bool Input::GetKey(uint32_t keyCode) const
    {
        return IsValidKeyCode(keyCode) && keyStates[keyCode].isKey;
    }
    bool Input::GetMouseButtonDown(uint32_t button) const
    {
        return IsValidMouseButton(button) && MouseButtonStates[button].isButtonDown;
    }
    bool Input::GetMouseButtonUp(uint32_t button) const
    {
        return IsValidMouseButton(button) && MouseButtonStates[button].isButtonUp;
    }
    bool Input::GetMouseButton(uint32_t button) const
    {
        return IsValidMouseButton(button) && MouseButtonStates[button].isButton;

    }
    void Input::OnKeyDown(uint32_t keyCode)
    {
        //키코드 값이 유효한지 확인
        if (!IsValidKeyCode(keyCode))
        {
            return;
        }

        //키 상태 가져오기
        KeyState& state = keyStates[keyCode];

        //눌림이 해제된 상태에서 처음 눌림 상태로 변경된 경우에만 down
        if (!state.isKey)
        {
            state.isKeyDown = true;
        }

        //현재 눌렸는지 여부
        state.isKey = true;
    }
    void Input::OnKeyUp(uint32_t keyCode)
    {
        //키코드 값이 유효한지 확인
        if (!IsValidKeyCode(keyCode))
        {
            return;
        }

        //키 상태 가져오기
        KeyState& state = keyStates[keyCode];

        //키가 눌린 상태에서 처음 해제될 때 up 설정
        if (state.isKey)

        {
            state.isKeyUp = true;
        }

        //눌림 해제 상태로 설정
        state.isKey = false;
    }
    void Input::OnMouseButtonDown(uint32_t button)
    {
        //버튼 코드가 유효한지 확인
        if (!IsValidMouseButton(button))
        {
            return;
        }

        //버튼 인덱스에 해당하는 상태 가져오기
        MouseButtonState& state = MouseButtonStates[button];

        //눌림이 해제된 상태에서만 눌림 기록
        if (!state.isButton)
        {
            state.isButtonDown = true;
        }

        state.isButton = true;

    }
    void Input::OnMouseButtonUp(uint32_t button)
    {

        //버튼 코드가 유효한지 확인
        if (!IsValidMouseButton(button))
        {
            return;
        }

        //버튼 인덱스에 해당하는 상태 가져오기
        MouseButtonState& state = MouseButtonStates[button];

        //눌림이 해제된 상태에서만 눌림 기록
        if (state.isButton)
        {
            state.isButtonUp = true;
        }

        state.isButton = false;
    }
    void Input::SetMousePosition(int x, int y)
    {
        //전달된 값을 현재 위치로 설정
        mousePosition = Vector2(static_cast<float>(x), static_cast<float>(y));
    }
    void Input::ResetAll()
    {
        //모든 키 입력 초기화
        for (KeyState& state : keyStates)
        {
            state.isKeyDown = false;
            state.isKeyUp = false;
        }

        
        //모든 마우스 입력 초기화
        for (MouseButtonState& state : MouseButtonStates)
        {
            state.isButtonDown = false;
            state.isButtonUp = false;
        }

        //마우스 위치 초기화
        mousePreviousPosition = mousePosition;

    }
    void Input::ResetInputs()
    {
        // 프레임 간의 입력 차이를 위한 초기화
        for (KeyState& state : keyStates)
        {
            state.isKeyDown= false;
            state.isKeyUp = false;
        }
    }
    bool Input::IsValidKeyCode(uint32_t keyCode) const
    {
        return keyCode < keyStateCount;
    }
    bool Input::IsValidMouseButton(uint32_t button) const
    {
        return button < MouseButtonCount;
    }
}