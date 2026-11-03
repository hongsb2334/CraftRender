#include "Texture.h"
#include "Renderer.h"
#include <Core/Core.h>
#include <cassert>


#define STB_IMAGE_IMPLEMENTATION
#include <ThirdParty/STB_Image.h>   

namespace Craft
{
    Texture::Texture(const std::string& path)
    {
        LoadFromFile(path);

    }
    Texture::~Texture()
    {
        SafeRelease(texture);
        SafeRelease(shaderResourceView);
        SafeRelease(samplerState);

    }
    void Texture::Bind(uint32_t slot) const
    {
        auto& context = Renderer::Get().GetContext();

        //텍스처 바인딩
        context.PSSetShaderResources(slot, 1, &shaderResourceView);

        //샘플러 바인딩
        context.PSSetSamplers(slot, 1, &samplerState);
    }
    std::shared_ptr<Texture> Texture::Load(const std::string& path)
    {
        return std::make_shared<Texture>(path);
    }
    void Texture::LoadFromFile(const std::string& path)
    {
        int imageWidth = 0;
        int imageHeight = 0;
        int channelCount = 0;

        //이미지 로드해서 픽셀 데이터(배열) 저장
        stbi_uc* pixels = stbi_load(path.c_str(), &imageWidth, &imageHeight, &channelCount, STBI_rgb_alpha);

        //pixels == nullptr이면 로드 실패
        if(pixels == nullptr)
        {
            assert(false);
            return;
        }

        width = static_cast<uint32_t>(imageWidth);
        height = static_cast<uint32_t>(imageHeight);

        //d3d 리소스 생성
        auto& device = Renderer::Get().GetDevice();

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA textureData = {};
        textureData.pSysMem = pixels;
        //texture 2차원 배열 가로 너비 설정
        textureData.SysMemPitch = width * 4;



        
        auto result = device.CreateTexture2D(&textureDesc, &textureData, &texture);
            

        //이미지 로드한 메모리 해제
        stbi_image_free(pixels);
        
        //성공/실패 여부 확인.
        ThrowIfFailed(result, L"Failed to create texture2d");

        //셰이더 리소스 뷰 생성
        D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc = {};
        srvdesc.Format = textureDesc.Format;
        srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvdesc.Texture2D.MipLevels = 1;
        srvdesc.Texture2D.MostDetailedMip = 0;

        ThrowIfFailed(
            device.CreateShaderResourceView(texture, &srvdesc, &shaderResourceView), L"Failed to create shader resource view"
        );

        //샘플러 스테이트
        // - 텍스처를 어떤 방식으로 읽을 지(샘플링 할지)를 결정하는 객체
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        //Anisotropy 안쓰겠다는 의미로 1
        samplerDesc.MaxAnisotropy = 1;

        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        ThrowIfFailed(
            device.CreateSamplerState(&samplerDesc, &samplerState), 
            L"Failed to Create Sampler State"
        );

    }
}

