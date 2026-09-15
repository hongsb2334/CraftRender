#pragma once
#include <Windows.h>

namespace Craft
{

    //window에서 engine으로 메시지 처리를 전달할 때 사용할 인터페이스
    class IMessageHandler
    {   
    public:
        //순수 가상 함수
        
        virtual LRESULT HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam) = 0;

    };
}