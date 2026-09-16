#include "Engine.h"
#include "Core/Win32Window.h"
#include <Graphics/Renderer.h>

namespace Craft
{
    Engine::Engine(uint32_t width, uint32_t height, const std::wstring title)
    {
        //창 객체 생성
        //this넘기는건 부모인 인터페이스로 업캐스팅해서 넘김
        window = std::make_unique<Win32Window>(width, height, this, title);

        //렌더러 객체 생성
        renderer = std::make_unique<Renderer>(*window);
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

    LRESULT Engine::HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
    {
        //엔진이 윈도우를 소유
        //윈도우가 엔진에 접근하려면 인터페이스 사용, 상호 참조하는것은 좋지 않음
        //메시지 처리
        switch (message)
        {
            //창 닫기 메시지 (x 버튼 누르면 발생)
        case WM_CLOSE:
        {
            //창 객체 삭제
            DestroyWindow(window);
        }
        return 0;
        //창 삭제 이벤트 처리
        case WM_DESTROY:
        {
            // 프로그램 종료 요청.(종료 메시지 발행) return 0; 에서의 0
            PostQuitMessage(0);
        }
        return 0;

        //ESC키 입력 처리
        case WM_KEYDOWN:
        {
            //눌린 키가 ESC키인지 확인
            if (wparam == VK_ESCAPE)
            {
                DestroyWindow(window);
            }
        }
        return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(window, &ps);
        }
        return 0;
        }
        return DefWindowProc(window, message, wparam, lparam);
    }
}

