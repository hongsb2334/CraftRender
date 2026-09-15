#include "Renderer.h"
#include <cstdint>

namespace Craft
{
    Renderer::Renderer()
    {
        CreateDevices();
    }

    Renderer::~Renderer()
    {

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
    }
}