#include "Component.h"

namespace Craft
{
	Component::Component()
	{
	}

	void Component::BeginPlay()
	{
		// 플래그 설정.
		hasBeganPlay = true;
	}

	void Component::Tick(float deltaTime)
	{
	}

	void Component::Draw()
	{
	}

	void Component::Destroy()
	{
		// 삭제 요청 플래그 설정.
		hasExpired = true;
		OnDestroyed();
	}

	void Component::OnDestroyed()
	{
	}
}