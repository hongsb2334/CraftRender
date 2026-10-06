#include "Renderer.h"
#include <Core/Win32Window.h>
#include <cstdint>
#include <d3dcompiler.h>
#include <cstring>
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

        //뷰포트 생성 및 바인딩
        CreateViewPort(window.GetWidth(), window.GetHeight());
    
        //트랜스폼 버퍼 생성
        CreateTransformBuffer();
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

        SafeRelease(transformBuffer);
        
    }

    void Renderer::Draw(float red, float green, float blue, uint32_t vsync)
    {
        BeginScene(red, green, blue);
        DrawScene();
        EndScene(vsync);
    }

    void Renderer::OnResize(uint32_t width, uint32_t height)
    {
        //원래 크기 확인
        DXGI_SWAP_CHAIN_DESC desc = {};
        swapChain->GetDesc(&desc);


        //렌더 타겟뷰 해제
        //백버퍼가 렌더 타겟뷰랑 연결되어 있어서 끊어줌
        SafeRelease(renderTargetView);

        //백버퍼 크기 변경
        ThrowIfFailed(swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, 0), L"Failed to resize back buffer");


        swapChain->GetDesc(&desc);


        //렌더 타겟뷰 재생성
        CreateRenderTargetView();


        //백버퍼 크기 변경되었으니까 뷰포트 크기 재설정
        CreateViewPort(width, height);

        

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
        
        //위 객체와 일대일 대응이 되야 함. 레이아웃이 바뀌면 코드를 매번 바꿔야 하는데, 이걸 자동화를 어떻게 할 것인지 고민
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

    void Renderer::CreateViewPort(uint32_t width, uint32_t height)
    {
        //뷰포트 설정
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        //바인딩
        context->RSSetViewports(1, &viewport);
    }

    void Renderer::CreateTransformBuffer()
    {
        //버퍼 구성 정보
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(Matrix4);
        //언리얼의 다이나믹 머티리얼 인스턴스임
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        //cpu에서 쓰고 gpu에서 읽는다
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA vertexBufferData = {};
        vertexBufferData.pSysMem = Matrix4::Identity.Data();

        ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc,
            &vertexBufferData,
            &transformBuffer), L"Failed to create transform buffer");
    }

    void Renderer::UpdateTransformBuffer(const Matrix4& worldMatrix)
    {
        //버퍼에 저장할 데이터 설정 과정 처리
        //아래 함수로 일반 버퍼의 데이터 변경 가능
        //간헐적인(너무 자주는 아니고) 데이터 갱신 시 사용 권장
        //context->UpdateSubresource();

        //버퍼와 연결할 리소스 생성
        D3D11_MAPPED_SUBRESOURCE mapped = {};
        
        //버퍼와 연동
        //hresult로 반환하므로 throwiffailed 해주기
        ThrowIfFailed(context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped), L"failed to map transform buffer");

        //업데이트할 데이터 설정
        std::memcpy(mapped.pData, worldMatrix.Data(), sizeof(Matrix4));

        //버퍼와 연동 해제
        context->Unmap(transformBuffer, 0);
    }
    
}