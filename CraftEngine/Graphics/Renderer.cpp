#include "Renderer.h"
#include "StaticMesh.h"
#include "Shader.h"
#include <Core/Win32Window.h>
#include <cstdint>
#include <d3dcompiler.h>
#include <cstring>
#include <cassert>

namespace Craft
{
    Renderer::Renderer(const Win32Window& window)
    {
        //처음 생성할 때는 instance가 nullptr이어야 함
        assert(!instance);
        
        //전역 접근 변수 설정        
        instance = this;

        //Device/Context 생성
        CreateDevices();

        //스왑체인 생성
        CreateSwapChain(window);

        //렌더 타겟 뷰 생성
        CreateRenderTargetView();
        
        //뷰포트 생성 및 바인딩
        CreateViewPort(window.GetWidth(), window.GetHeight());
    
        //트랜스폼 버퍼 생성
        CreateTransformBuffer();

        //트랜스폼 버퍼 생성
        CreateTransformBuffer();

        //카메라 버퍼 생성
        CreateCameraBuffer();
    }

    Renderer::~Renderer()
    {
        instance = nullptr;

        //리소스 해제
        
        SafeRelease(transformBuffer);
        SafeRelease(cameraBuffer);


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

    void Renderer::Submit(const std::shared_ptr<StaticMesh>& mesh, const std::shared_ptr<Shader>& shader, const Matrix4& worldMatrix)
    {
        //삽입할 렌더 명령 생성
        RenderCommand command;
        command.worldMatrix = worldMatrix;
        command.mesh = mesh;
        command.shader = shader;

        //렌더 목록에 추가
        renderCommandList.emplace_back(command);
    }

    void Renderer::SubmitCamera(const Matrix4& viewMatrix, const Matrix4& projectionMatrix)
    {
        cameraData.viewMatrix = viewMatrix;
        cameraData.projectionMatrix = projectionMatrix;
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

    Renderer& Renderer::Get()
    {
        assert(instance && "instance should not be null");
        return *instance;
        // TODO: insert return statement here
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
        //정점 셰이더의 상수 버퍼(트랜스폼 버퍼) 바인딩
        context->VSSetConstantBuffers(0, 1, &transformBuffer);
        
        //카메라 버퍼 업데이트
        UpdateCameraBuffer();
        
        //정점 셰이더의 상수 버퍼(카메라 버퍼) 바인딩
        context->VSSetConstantBuffers(1, 1, &cameraBuffer);

        //렌더 명령 처리
        for (const RenderCommand& command : renderCommandList)
        {
            DrawCommand(command);
        }

        //렌더 명령 목록 정리
        renderCommandList.clear();


        
    }

    void Renderer::DrawCommand(const RenderCommand& command)
    {
        //메시 or 셰이더 설정이 안됐으면 종료
        if (!command.mesh|| !command.shader)
        {
            return;
        }

        //메시 바인딩
        command.mesh->Bind();

        command.shader->Bind();

        //트랜스폼 데이터 업데이트
        UpdateTransformBuffer(command.worldMatrix);

        //드로우 콜
        context->DrawIndexed(command.mesh->GetIndexCount(), 0, 0);
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
        //swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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

    void Renderer::CreateStencilView(uint32_t width, uint32_t height)
    {
        //뎁스/스텐실 버퍼를 생성하기 위한 설명 구조체(Descriptor
        D3D11_TEXTURE2D_DESC depthBufferDesc = {};
        depthBufferDesc.Width = width;
        depthBufferDesc.Height= height;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality= 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        ThrowIfFailed(device->CreateTexture2D(
            &depthBufferDesc,
            nullptr,
            &depthStencilBuffer),
            L"Failed to create depth stencil buffer"
        );




        //device->CreateTexture2D(depthBufferDesc, );
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

    void Renderer::CreateCameraBuffer()
    {
        //버퍼 구성 정보
        D3D11_BUFFER_DESC BufferDesc = {};
        BufferDesc.ByteWidth = sizeof(CameraBufferData);
        //언리얼의 다이나믹 머티리얼 인스턴스임
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        //cpu에서 쓰고 gpu에서 읽는다
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA BufferData = {};
        BufferData.pSysMem = &cameraData;

        ThrowIfFailed(device->CreateBuffer(&BufferDesc,
            &BufferData,
            &cameraBuffer), L"Failed to create camera buffer");

    }

    void Renderer::UpdateCameraBuffer()
    {
        //버퍼와 연결할 리소스 생성
        D3D11_MAPPED_SUBRESOURCE mapped = {};

        //버퍼와 연동
        //hresult로 반환하므로 throwiffailed 해주기
        ThrowIfFailed(context->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped), L"failed to map camera buffer");

        //업데이트할 데이터 설정
        std::memcpy(mapped.pData, &cameraData, sizeof(CameraBufferData));

        //버퍼와 연동 해제
        context->Unmap(cameraBuffer, 0);

    }
    
}