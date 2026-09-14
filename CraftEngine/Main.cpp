#include <Windows.h>
#include "LaunchApplication.h"

#if _DEBUG
int main()
{
    return LaunchApplication();
}
#else 

//윈도우즈 모드에서 사용하는 메인 함수
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    return LaunchApplication();
}
#endif
