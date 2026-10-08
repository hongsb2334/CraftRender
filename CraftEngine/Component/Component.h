#pragma once

#include <memory>

namespace Craft
{
	// 전방선언.
	class Actor;

	// 컴포넌트 중에 최상위 클래스.
	class Component
	{
	public:
		Component();
		virtual ~Component() = default;

		// 게임플레이 관련 이벤트.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 삭제 함수 및 이벤트 함수.
		void Destroy();
		virtual void OnDestroyed();

		// Getter/Setter.
		inline std::shared_ptr<Actor> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Actor> newOwner) { owner = newOwner; }
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

	protected:
		// 오너십 - 이 컴포넌트를 소유하는 액터.
		std::weak_ptr<Actor> owner;

		// BeginPlay 호출 여부 확인 플래그.
		bool hasBeganPlay = false;

		// 컴포넌트 활성화 여부 플래그.
		bool isActive = true;

		// 삭제 요청 여부를 확인하는 플래그.
		bool hasExpired = false;
	};
}