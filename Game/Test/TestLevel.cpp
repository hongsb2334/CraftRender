#include "TestLevel.h"
#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Renderer.h>
#include <Math/Matrix4.h>

using namespace Craft;
TestLevel::TestLevel()
{
}

void TestLevel::Initialized()
{
    Level::Initialized();

    //리소스 생성
    //종류별로 한개씩만 로드해서 필요할 때 재사용
    auto triangle = StaticMesh::CreateTriangleMesh();
    auto quad = StaticMesh::CreateQuadMesh();

    //셰이더 생성 
    auto shader = Shader::CreateDefaultShader();

    //액터 생성
    auto left = SpawnActor<TestActor>();
    left->GetTransform().scale = Vector3::One * 0.4f;
    left->GetTransform().position.x = -0.5f;
    
    
    //액터에 스태틱 메시 설정
    left->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
    
    //셰이더 설정
    left->GetComponent<StaticMeshComponent>()->SetShader(shader);

    
    auto right = SpawnActor<TestActor>();
    right->GetTransform().scale = Vector3::One * 0.4f;
    right->GetTransform().position.x = 0.5f;

    


    //액터에 삼각형 메시 설정
    right->GetComponent<StaticMeshComponent>()->SetStaticMesh(triangle);

    right->GetComponent<StaticMeshComponent>()->SetShader(shader);

    auto center = SpawnActor<TestActor>();
    center->GetTransform().scale = Vector3::One * 0.4f;
    center->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);

    center->GetComponent<StaticMeshComponent>()->SetShader(shader);

}

void TestLevel::Draw()
{
    Level::Draw();

    // 뷰 행렬 제출

    Vector3 eye(0.0f, 0.0, 0.0f);
    Vector3 target = eye + Vector3::Forward;
    Matrix4 view = Matrix4::LookAt(eye, target, Vector3::Up);

    Renderer::Get().SubmitCamera(view);
}
