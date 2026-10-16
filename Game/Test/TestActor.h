#pragma once
#include <GameFramework/Actor.h>



class TestActor : public Craft::Actor
{
public:
    virtual void Initialized() override;
    virtual void Tick(float deltaTime) override;
};

