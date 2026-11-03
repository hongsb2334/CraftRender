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
    void Input::ResetAll()
    {
        //모든 키 입력 초기화
        for (KeyState& state : keyStates)
        {
            state.isKeyDown = false;
            state.isKeyUp = false;
            state.isKey = false;
        }
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
}