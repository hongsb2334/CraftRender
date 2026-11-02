#pragma once

#include <Component/Component.h>
#include <memory>
namespace Craft
{
    //전방 선언
    class StaticMesh;
    class Material;

    class StaticMeshComponent : public Component
    {
    public:
        StaticMeshComponent();
        ~StaticMeshComponent();

        virtual void Draw() override;



        //setter
        inline void SetStaticMesh(const std::shared_ptr<StaticMesh>& newMesh)
        {
            staticMesh = newMesh;
        }
        inline void SetMaterial(const std::shared_ptr<Material>& newMaterial)
        {
            material = newMaterial;
        }
       

    private:
        //스태틱 메시
        std::shared_ptr<StaticMesh> staticMesh;

        //머티리얼
        std::shared_ptr<Material> material;
    };
}


