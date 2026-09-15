#include "Win32Window.h"
#include <Interface/IMessageHandler.h>
namespace Craft
{
    Win32Window::Win32Window(uint32_t width, uint32_t height, IMessageHandler* messageHandler, const std::wstring title) :
        width(width), height(height), title(title), messageHandler(messageHandler), instance(GetModuleHandle(nullptr))
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
            this        // Additional application data
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
        //생성 이벤트
        if (message == WM_CREATE)
        {
            //윈도우 파라미터 설정
            //생성할 때 넘겨줬던 추가 파라미터 가져오기
            CREATESTRUCT* createstruct = reinterpret_cast<CREATESTRUCT*>(lparam);
            if (createstruct)
            {
                //임시 저장
                Win32Window* win32window = reinterpret_cast<Win32Window*>(createstruct->lpCreateParams);

                if (win32window && win32window->messageHandler)
                {
                    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)win32window);
                }
            }

            return 0;
        }
        
        //생성 이후의 이벤트 처리
        Win32Window* win32window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
        if (win32window && win32window->messageHandler)
        {
            //이벤트를 인터페이스를 통해서 전달
            return win32window->messageHandler->HandleMessage(window, message, wparam, lparam);
            
        }

        //방어 코드
        return DefWindowProc(window, message, wparam, lparam);

    }
}