#pragma once
#include <Math/Vector3.h>
#include <d3d11.h>
#include <cstdint>
#include <vector>
#include <memory>


namespace Craft
{
    class StaticMesh
    {
    public:
        StaticMesh(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices);
        ~StaticMesh();

        //바인딩 함수 - 버퍼(리소스)를 그래픽스 파이프라인에 설정
        void Bind() const;

        //편의 함수
        static std::shared_ptr<StaticMesh> CreateTriangleMesh();
 

        //getter
        inline uint32_t GetIndexCount() const { return indexCount; }

    private:
        //정점 버퍼
        ID3D11Buffer* vertexBuffer = nullptr;
        
        //인덱스 버퍼
        ID3D11Buffer* indexBuffer = nullptr;

        //인덱스 개수
        uint32_t indexCount = 0;

        //정점 데이터 바이트 너비 
        uint32_t stride = 0;

    };
}


