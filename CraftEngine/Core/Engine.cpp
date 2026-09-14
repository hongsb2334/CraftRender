#include "Engine.h"
#include <Core/Win32Window.h>

namespace Craft
{
    Engine::Engine(uint32_t width, uint32_t height, const std::wstring title)
    {
        //창 객체 생성
        window = std::make_unique<Win32Window>(width, height, title);
    }

    Engine::~Engine()
    {
    }

    void Engine::Run()
    {
        

        //이벤트(창 메시지 처리 루프)
        MSG message = {};
        while (message.message != WM_QUIT)
        {
            //min/max설정 해도되고 0 넣어도 다 받아줌
            //창에 메시지가 발생한 경우의 처리
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            //엔진 루프 처리 등
            else
            {

            }
        }

    }

    void Engine::Quit()
    {
    }
}

