#include "Renderer.h"
#include <Core/Win32Window.h>
#include <cstdint>
#include <d3dcompiler.h>
namespace Craft
{
    Renderer::Renderer(const Win32Window& window)
    {
        //Device/Context 생성
        CreateDevices();

        //스왑체인 생성
        CreateSwapChain(window);

        //렌더 타겟 뷰 생성
        CreateRenderTargetView();

        //데모 버퍼 생성
        CreateDemoBuffers();
        
        //셰이더 컴파일 및 셰이더 객체 생성
        CreateDefaultShaders();
    }

    Renderer::~Renderer()
    {
        SafeRelease(vertexBuffer);
        SafeRelease(indexBuffer);
        SafeRelease(vertexShader);
        SafeRelease(pixelShader);
        SafeRelease(inputLayout);

        //리소스 해제
        //장치(그래픽카드) 관련
        SafeRelease(renderTargetView);
        SafeRelease(swapChain);   
        SafeRelease(context);
        SafeRelease(device);


        
    }

    void Renderer::Draw(float red, float green, float blue, uint32_t vsync)
    {
        BeginScene(red, green, blue);
        DrawScene();
        EndScene(vsync);
    }

    void Renderer::BeginScene(float red, float green, float blue)
    {
        //그리기 준비
        //배경 지우기 및 그리기 대상 설정, 드로우콜 했을 때 어디다 그릴지 설정
        context->OMSetRenderTargets(1, &renderTargetView, nullptr);

        //배경을 지우는 것은 그냥 단색으로 다 채우는 것임
        const float backGroundColor[4] = { red, green, blue, 1.0f };
        context->ClearRenderTargetView(renderTargetView, backGroundColor);
    }

    void Renderer::DrawScene()
    {
        //입력 설정 - 리소스 바인딩
        uint32_t stride = sizeof(float) * 3;
        uint32_t offset = 0;
        context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        context->IASetInputLayout(inputLayout);

        //토폴로지->정점을 어떻게 이어서 도형을 만들지 결정하는 방식
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
        //셰이더 설정
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        //드로우 콜
        context->DrawIndexed(3, 0, 0);

        // 뷰포트 설정.
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = 1280.0f;
        viewport.Height = 800.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        context->RSSetViewports(1, &viewport);
        
    }

    void Renderer::EndScene(uint32_t vsync)
    {
        //프론트-백 버퍼 교환
        swapChain->Present(vsync, 0);
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

        ThrowIfFailed(D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flag,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &device,
            &selectedFeatureLevel,  //nullptr 넘겨도 됨
            &context
        ), L"Failed to create device");

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

    void Renderer::CreateRenderTargetView()
    {
        //백버퍼(2차원 배열 - 텍스처) 정보 가져오기
        ID3D11Texture2D* backbuffer = nullptr;
        ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)), L"Failed to get back buffer from swap chain");

        //렌더 타겟 뷰 생성
        ThrowIfFailed(device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView), L"Failed to create RTV");


        //사용한 후 해제
        SafeRelease(backbuffer);
    }

    void Renderer::CreateDemoBuffers()
    {
        //Temp: 구조체 선언
        
        struct Vector3
        {
            float x, y, z = 0.0f;
        };

        //삼각형을 이루는 정점 데이터(배열)
        Vector3 vertices[] = {
            Vector3 {0.0f, 0.5f, 0.5f},
            Vector3 {0.5f, -0.5f, 0.5f},
            Vector3 {-0.5f, -0.5f, 0.5f},
        };

        //원시 데이터를 포장해서 그래픽카드에 전달(사실 복사임)해야 함
        //전달 매개체가 버퍼
        
        //버퍼 구성 정보
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(Vector3) * 3;
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA vertexBufferData = {};
        vertexBufferData.pSysMem = vertices;

        ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, 
            &vertexBufferData,
            &vertexBuffer), L"Failed to create vertex buffer");




        //인덱스 원시 데이터 배열
        //정점의 순서 - 삼각형을 구성할 인덱스 순서
        uint32_t indices[] = {0, 1, 2};
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(uint32_t) * 3;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA indexBufferData = {};
        indexBufferData.pSysMem = indices;

        ThrowIfFailed(device->CreateBuffer(&indexBufferDesc,
            &indexBufferData,
            &indexBuffer), L"Failed to create index buffer");
    }

    void Renderer::CreateDefaultShaders()
    {
        //셰이더 컴파일 결과 저장용 객체
        ID3DBlob* vertexShaderObject = nullptr;

        //셰이더(Shader) 컴파일
        ThrowIfFailed(D3DCompileFromFile(
            L"HLSLShaders/DefaultVS.hlsl",
            nullptr,
            nullptr,
            "main",
            "vs_5_0",
            0,
            0,
            &vertexShaderObject,
            nullptr

        ), L"Failed to compile vertex shader");

        //정점 셰이더 객체 생성
        ThrowIfFailed(device->CreateVertexShader(vertexShaderObject->GetBufferPointer(),
            vertexShaderObject->GetBufferSize(),
            nullptr,
            &vertexShader),
            L"Failed to create vertex shader");


        /*
        LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
        */

        //정점 셰이더 입력 관련 정보 객체 생성
        D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        
        ThrowIfFailed(device->CreateInputLayout(inputLayoutDesc, _countof(inputLayoutDesc), vertexShaderObject->GetBufferPointer(),
            vertexShaderObject->GetBufferSize(),
            &inputLayout), L"failed to create input layout");


        ID3DBlob* pixelShaderObject = nullptr;
        //픽셀 셰이더

        //셰이더(Shader) 컴파일
        ThrowIfFailed(D3DCompileFromFile(
            L"HLSLShaders/DefaultPS.hlsl",
            nullptr,
            nullptr,
            "main",
            "ps_5_0",
            0,
            0,
            &pixelShaderObject,
            nullptr
        ), L"Failed to compile pixel shader");

        //정점 셰이더 객체 생성
        ThrowIfFailed(device->CreatePixelShader(pixelShaderObject->GetBufferPointer(),
            pixelShaderObject->GetBufferSize(),
            nullptr,
            &pixelShader),
            L"Failed to create pixel shader");


        //사용한 리소스 해제
        SafeRelease(vertexShaderObject);
        SafeRelease(pixelShaderObject);
            
    }
    
}