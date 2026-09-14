#include "Win32Window.h"

namespace Craft
{
    Win32Window::Win32Window(uint32_t width, uint32_t height, const std::wstring title) : 
        width(width), height(height), title(title), instance(GetModuleHandle(nullptr))
    {
        //창 만들기
        // Register the window class.

        WNDCLASS wc = { };

        //창 만들때 사용되는 정보 구조체
        wc.lpfnWndProc = Win32MessageHandler;
        wc.hInstance = instance;
        wc.lpszClassName = className.c_str();
        wc.style = CS_HREDRAW | CS_VREDRAW;

        //클래스 등록 전달되는 클래스 이름은 고유값이어야 함
        if (!RegisterClass(&wc))
        {
            //디버그 모드일때 강제 중단해주는 함수
            //assert 써도 됨
            __debugbreak();
            return;
        }


        //창 크기 조정
        RECT rect = { 0, 0, (long)width, (long)height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        //창 크기 계산
        uint32_t windowWidth = rect.right - rect.left;
        uint32_t windowHeight = rect.bottom - rect.top;

        //화면 가운데 창 생성하도록 위치 구하기
        uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
        uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

        // Create the window.

        handle = CreateWindow(
            className.c_str(),                     // Window class
            title.c_str(),                      // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // position and Size  
            x, y, windowWidth, windowHeight,

            nullptr,       // Parent window    
            nullptr,       // Menu
            instance,     // Instance handle
            nullptr        // Additional application data
        );

        if (!handle)
        {
            __debugbreak();
            return;
        }

        //GetClientRect(hwnd, &rect);


        //창 보이기 모드 설정
        ShowWindow(handle, SW_SHOW);
    }
    Win32Window::~Win32Window()
    {
        //클래스 등록 해제
        UnregisterClass(className.c_str(), instance);
    }
    LRESULT Win32Window::Win32MessageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
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