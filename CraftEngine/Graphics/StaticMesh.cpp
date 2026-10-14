#include "StaticMesh.h"
#include "Renderer.h"
#include <Core/Core.h>
#include <cassert>

namespace Craft
{
    StaticMesh::StaticMesh(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices)
    {
        //정점/ 인덱스 배열의 항목 수가 0보다 큰지 확인
        assert(vertices.size() > 0);
        assert(indices.size() > 0);

        //인덱스 개수 설정
        indexCount = static_cast<uint32_t>(indices.size());


        //정점데이터 바이트 너비 설정
        stride = sizeof(vertices[0]);

        //장치 얻어오기
        auto& device = Renderer::Get().GetDevice();

        //원시 데이터를 포장해서 그래픽카드에 전달(사실 복사임)해야 함
        //전달 매개체가 버퍼

        //버퍼 구성 정보
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = static_cast<uint32_t>(stride * vertices.size());
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA vertexBufferData = {};
        //.data로 원시포인터 반환
        vertexBufferData.pSysMem = vertices.data();

        ThrowIfFailed(device.CreateBuffer(&vertexBufferDesc,
            &vertexBufferData,
            &vertexBuffer), L"Failed to create vertex buffer");


        //인덱스 원시 데이터 배열
        //정점의 순서 - 삼각형을 구성할 인덱스 순서
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(indices.size());
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        //버퍼에 저장할 데이터
        D3D11_SUBRESOURCE_DATA indexBufferData = {};
        indexBufferData.pSysMem = indices.data();

        ThrowIfFailed(device.CreateBuffer(&indexBufferDesc,
            &indexBufferData,
            &indexBuffer), L"Failed to create index buffer");


    }



    StaticMesh::~StaticMesh()
    {
        //리소스 해제
        SafeRelease(vertexBuffer);
        SafeRelease(indexBuffer);
        


    }
    void StaticMesh::Bind() const
    {
        //컨텍스트 가져오기
        auto& context = Renderer::Get().GetContext();


        uint32_t offset = 0;
        context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        //토폴로지->정점을 어떻게 이어서 도형을 만들지 결정하는 방식
        context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    std::shared_ptr<StaticMesh> StaticMesh::CreateTriangleMesh()
    {

        return std::make_shared<StaticMesh>(
            std::vector<Vector3>{
            Vector3(0.0f, 0.5f, 0.5f),
            Vector3(0.5f, -0.5f, 0.5f),
                Vector3(-0.5f, -0.5f, 0.5f)
        },
            std::vector<uint32_t>{ 0,1,2 }
        );
    }
    std::shared_ptr<StaticMesh> StaticMesh::CreateQuadMesh()
    {


        return std::make_shared<StaticMesh>(
            std::vector<Vector3> {
            Vector3(-0.5f, 0.5f, 0.5f),
            Vector3(0.5f, 0.5f, 0.5f),
            Vector3(0.5f, -0.5f, 0.5f),
            Vector3(-0.5f, -0.5f, 0.5f),
        }, 
            //왼쪽 위부터 시계방향으로 0,1,2,3으로 가정
            std::vector<uint32_t>{0, 1, 2, 0, 2, 3}
        );
    }
}