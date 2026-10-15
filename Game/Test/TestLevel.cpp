#include "TestLevel.h"
#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/Shader.h>

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
    left->SetStaticMesh(quad);
    
    //셰이더 설정
    left->SetShader(shader);

    
    auto right = SpawnActor<TestActor>();
    right->GetTransform().scale = Vector3::One * 0.4f;
    right->GetTransform().position.x = 0.5f;

    


    //액터에 삼각형 메시 설정
    right->SetStaticMesh(triangle);

    right->SetShader(shader);

    auto center = SpawnActor<TestActor>();
    center->GetTransform().scale = Vector3::One * 0.4f;
    center->SetStaticMesh(quad);

    center->SetShader(shader);

}
