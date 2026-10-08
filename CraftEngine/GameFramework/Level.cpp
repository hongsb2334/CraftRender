#include "Level.h"

namespace Craft
{
	Level::Level()
	{
	}

	void Level::Initialized()
	{
	}

	void Level::BeginPlay()
	{
		// 액터를 순회하면서 액터의 BeginPlay 호출.
		for (const auto& actor : actorList)
		{
			// 액터가 활성화 상태가 아니거나 이미 BeginPlay 처리가 된 경우에는 건너뛰기.
			if (!actor->IsActive() || actor->HasBeganPlay())
			{
				continue;
			}

			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		for (const auto& actor : actorList)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Tick(deltaTime);
		}
	}

	void Level::Draw()
	{
		for (const auto& actor : actorList)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Draw();
		}
	}

	void Level::ProcessAddAndDestroyActors()
	{
		// 액터의 컴포넌트 추가/제거 처리.
		for (const auto& actor : actorList)
		{
			actor->ProcessAddAndDestroyComponents();
		}

		// 삭제 요청된 액터 제거 처리.
		for (auto iterator = actorList.begin(); iterator != actorList.end();)
		{
			// 삭제 요청 여부 확인.
			if ((*iterator)->HasExpired())
			{
				iterator = actorList.erase(iterator);
				continue;
			}

			++iterator;
		}

		// 액터 추가 요청 처리.
		if (addRequestedActorList.empty())
		{
			return;
		}

		for (const auto& actor : addRequestedActorList)
		{
			actor->ProcessAddAndDestroyComponents();
			actorList.emplace_back(actor);
		}

		// 추가 처리한 목록 정리.
		addRequestedActorList.clear();
	}
}