#pragma once

#include <Component/Component.h>

#include <memory>
#include <vector>

namespace Craft
{
	// 전방선언.
	class Level;
    class Transform;
    class TransformComponent;

	// 모든 하위 Actor 클래스의 최상위 클래스.
	class Actor : public std::enable_shared_from_this<Actor>
	{
		// Level 클래스를 friend로 선언.
		friend class Level;

	public:
		Actor();
		virtual ~Actor();

		// 게임 플레이 이벤트 함수.
		virtual void Initialized();
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 게임 종료 요청 함수.
		void QuitGame();

		// 삭제 요청 함수 및 이벤트 함수.
		void Destroy();
		virtual void OnDestroyed();

		// 컴포넌트 추가 함수.
		template<typename T, typename ...Args,
			typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
			std::shared_ptr<T> AddComponent(Args&& ...args)
		{
			// 컴포넌트 객체 생성.
			std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);

			// 오너십 설정.
			newComponent->SetOwner(weak_from_this());

			// 추가 요청 목록에 포함.
			addRequestedComponentList.emplace_back(newComponent);

			// 반환.
			return newComponent;
		}

		// 컴포넌트 검색 함수.
		template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
		std::shared_ptr<T> GetComponent()
		{
			// 컴포넌트 목록에서 찾기.
			for (const auto& component : componentList)
			{
				// 형변환 시도.
				std::shared_ptr<T> targetComponent
					= std::dynamic_pointer_cast<T>(component);
				if (targetComponent)
				{
					return targetComponent;
				}
			}

			for (const auto& component : addRequestedComponentList)
			{
				// 형변환 시도.
				std::shared_ptr<T> targetComponent
					= std::dynamic_pointer_cast<T>(component);
				if (targetComponent)
				{
					return targetComponent;
				}
			}

			// 찾지 못하면 null 반환.
			return nullptr;
		}

		// 특정 컴포넌트를 소유했는지 확인.
		template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
		bool HasComponent()
		{
			return GetComponent<T>() != nullptr;
		}

		// Getter/Setter.
		inline std::shared_ptr<Level> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

        Transform& GetTransform();
        const Transform& GetTransform() const;

	private:
		// 컴포넌트 추가/제거를 처리하는 함수.
		// 현재 프레임이 아니라 다음 프레임에서 안정적으로 처리하도록.
		void ProcessAddAndDestroyComponents();

	protected:
		// 오너십.
		std::weak_ptr<Level> owner;

		// BeginPlay 처리 여부 플래그.
		bool hasBeganPlay = false;

		// 활성화 여부 확인 플래그.
		bool isActive = true;

		// 삭제 요청이 됐는지 확인하는 플래그.
		bool hasExpired = false;

        //트랜스폼 컴포넌트 참조 함수
        std::shared_ptr<TransformComponent> transformComponent;



		// 컴포넌트 목록.
		std::vector<std::shared_ptr<Component>> componentList;

		// 추가 요청된 컴포넌트 목록 - 다음 프레임에 추가 처리.
		std::vector<std::shared_ptr<Component>> addRequestedComponentList;
	};
}