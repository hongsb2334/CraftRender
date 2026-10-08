#include "Actor.h"
#include <Component/TransformComponent.h>
#include <Core/Engine.h>
#include <cassert>

namespace Craft
{
	Actor::Actor()
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::Initialized()
	{   
        //액터 초기화할 때 트랜스폼 컴포넌트 추가
        if (!transformComponent)
        {
            transformComponent = AddComponent<TransformComponent>();
        }
	}

	void Actor::BeginPlay()
	{
		// 플래그 설정.
		hasBeganPlay = true;

		// 컴포넌트 처리.
		for (const auto& component : componentList)
		{
			if (!component->IsActive() || component->HasBeganPlay())
			{
				continue;
			}

			component->BeginPlay();
		}
	}

	void Actor::Tick(float deltaTime)
	{
		// 비활성화 시 종료.
		if (!IsActive())
		{
			return;
		}

		// 컴포넌트 처리.
		for (const auto& component : componentList)
		{
			if (!component->IsActive())
			{
				continue;
			}

			component->Tick(deltaTime);
		}
	}

	void Actor::Draw()
	{
		// 비활성화 시 종료.
		if (!IsActive())
		{
			return;
		}

		// 컴포넌트 처리.
		for (const auto& component : componentList)
		{
			if (!component->IsActive())
			{
				continue;
			}

			component->Draw();
		}
	}

	void Actor::QuitGame()
	{
        //엔진 종료 요청
        Engine::Get().Quit();


	}

	void Actor::Destroy()
	{
		// 삭제 요청 플래그 설정.
		hasExpired = true;

		// 삭제 이벤트 함수 호출.
		OnDestroyed();
	}

	void Actor::OnDestroyed()
	{
	}

	Transform& Actor::GetTransform()
	{
        assert(transformComponent);
        return transformComponent->GetTransform();
	}

	const Transform& Actor::GetTransform() const
	{
        assert(transformComponent);
        return transformComponent->GetTransform();
	}

	void Actor::ProcessAddAndDestroyComponents()
	{
		// 삭제 요청된 컴포넌트 목록 처리.
		for (auto iterator = componentList.begin(); iterator != componentList.end(); )
		{
			// 삭제 요청 여부 확인.
			if ((*iterator)->HasExpired())
			{
				iterator = componentList.erase(iterator);
				continue;
			}

			// 다음 이터레이터 처리.
			++iterator;
		}

		// 추가 요청된 목록 처리.
		if (addRequestedComponentList.empty())
		{
			return;
		}

		for (auto& component : addRequestedComponentList)
		{
			if (hasBeganPlay && component->IsActive())
			{
				component->BeginPlay();
			}

			componentList.emplace_back(component);
		}

		// 추가 요청 목록 정리.
		addRequestedComponentList.clear();
	}
}