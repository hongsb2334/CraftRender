#pragma once
#include <GameFramework/Actor.h>

//전방 선언
namespace Craft
{
    class StaticMesh;
}

class TestActor : public Craft::Actor   
{
public:
    virtual void Tick(float deltaTime) override;


private:
    //@Temp : 액터에서 사용할 스태틱 메시
    std::shared_ptr<Craft::StaticMesh> staticMesh;

};

