#pragma once

#include "Actor.h"
#include <memory>
#include <vector>

namespace Craft
{
	// 게임 공간에 배치된 모든 액터를 관리하는 클래스.
	class Level : public std::enable_shared_from_this<Level>
	{
		// Engine 클래스를 friend로 선언.
		friend class Engine;

	public:
		Level();
		virtual ~Level() = default;

		// 게임 플레이 이벤트 함수.
		virtual void Initialized();
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 액터 추가 함수.
		template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> SpawnActor(Args&& ...args)
		{
			// 새로운 액터 생성.
			std::shared_ptr<T> newActor
				= std::make_shared<T>(std::forward<Args>(args)...);

			// 액터 초기화 함수 호출.
			newActor->Initialized();

			// 오너십 설정.
			newActor->SetOwner(weak_from_this());

			// 추가 요청 목록에 새로운 액터 추가.
			addRequestedActorList.emplace_back(newActor);
			return newActor;
		}

		// 액터 검색 함수.
		template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> FindActor()
		{
			for (std::shared_ptr<T> actor : actorList)
			{
				std::shared_ptr<T> targetActor
					= std::dynamic_pointer_cast<T>(actor);
				if (targetActor)
				{
					return targetActor;
				}
			}

			for (std::shared_ptr<T> actor : addRequestedActorList)
			{
				std::shared_ptr<T> targetActor
					= std::dynamic_pointer_cast<T>(actor);
				if (targetActor)
				{
					return targetActor;
				}
			}

			// 검색에 실패하면 null 반환.
			return nullptr;
		}

	private:
		void ProcessAddAndDestroyActors();

	protected:
		// 액터 배열.
		std::vector<std::shared_ptr<Actor>> actorList;

		// 추가 요청된 액터 배열.
		std::vector<std::shared_ptr<Actor>> addRequestedActorList;
	};
}