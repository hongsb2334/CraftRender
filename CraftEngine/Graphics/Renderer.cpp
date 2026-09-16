#include "Renderer.h"
#include <Core/Win32Window.h>
#include <cstdint>

namespace Craft
{
    Renderer::Renderer(const Win32Window& window)
    {
        //Device/Context 생성
        CreateDevices();

        //스왑체인 생성
        CreateSwapChain(window);
    }

    Renderer::~Renderer()
    {
        //리소스 해제
        if (device)
        {
            device->Release();
            device = nullptr;
        }
        //리소스 해제
        if (context)
        {
            context->Release();
            context = nullptr;
        }

        if (swapChain)
        {
            swapChain->Release();
            swapChain = nullptr;
        }
        
    }

    void Renderer::CreateDevices()
    {
        uint32_t flag = 0;

#if _DEBUG
        flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        /*
        _In_opt_ IDXGIAdapter* pAdapter, -> 이거 안하면 주모니터가 메인 모니터
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext


        */

        //그래픽스 api 버전
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        //언리얼에서 그래픽 api 어떤거 쓰는지 알아낼때 사용(opengl이나 dx 등등)
        D3D_FEATURE_LEVEL selectedFeatureLevel = {};

        auto result = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flag,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &device,
            &selectedFeatureLevel,  //nullptr 넘겨도 됨
            &context
        );


        //실패 확인
        //FAILED 반대 SUCCEED도 있음
        if (FAILED(result))
        {
            __debugbreak();
            MessageBoxA(nullptr, "Failed to create device", "D3D Error", MB_OK);
        }

    }

    void Renderer::CreateSwapChain(const Win32Window & window)
    {
        //스왑체인 생성을 위한 객체 생성
        IDXGIFactory* factory = nullptr;

        //사실 c스타일이나 reinterpret 캐스트나 위험한건 매한가지라 아무거나 써도 됨
        //auto result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

        //매크로로 위 인자 전달한것 해줌
        auto result = CreateDXGIFactory(IID_PPV_ARGS(&factory));

        if (FAILED(result))
        {
            __debugbreak();
            //..
            return;
        }

        /*
        typedef struct DXGI_SWAP_CHAIN_DESC
    {
    DXGI_MODE_DESC BufferDesc;
    DXGI_SAMPLE_DESC SampleDesc;
    DXGI_USAGE BufferUsage;
    UINT BufferCount;
    HWND OutputWindow;
    BOOL Windowed;
    DXGI_SWAP_EFFECT SwapEffect;
    UINT Flags;
    } 	DXGI_SWAP_CHAIN_DESC;
        */

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = window.GetWidth();
        swapChainDesc.BufferDesc.Height = window.GetHeight();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.OutputWindow = window.GetHandle();
        //창모드 실행 여부
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        //스왑체인 생성
        result = factory->CreateSwapChain(device, &swapChainDesc, &swapChain);

        if (FAILED(result))
        {
            __debugbreak();
            return;
        }

        //해제
        if (factory)
        {
            factory->Release();
            factory = nullptr;
        }
    }
    
}