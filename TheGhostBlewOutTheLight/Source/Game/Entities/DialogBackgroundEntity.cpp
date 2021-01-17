#include "DialogBackgroundEntity.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/RenderComponent.h"
#include "GameEngine/EntitySystem/Entity.h"
#include <iostream>

using namespace Game;

DialogBackgroundEntity::DialogBackgroundEntity() :visible{ false } {
	spriteRenderer = AddComponent<GameEngine::RenderComponent>();
	spriteRenderer->SetFillColor(sf::Color(255, 255, 255,30));
	spriteRenderer->SetZLevel(9);

}

void DialogBackgroundEntity::Update()
{
	if (!visible) return;
	sf::Vector2f pos = GameEngine::CameraManager::GetInstance()->GetCameraView().getCenter();
	sf::Vector2u size = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow()->getSize();
	SetPos(sf::Vector2f(pos.x, pos.y + size.y / 3));
	SetSize(sf::Vector2f(size.x - 100, size.y / 3 - 50));
}

void DialogBackgroundEntity::OnAddToWorld() {}

void DialogBackgroundEntity::show() {
	visible = true;
}

void DialogBackgroundEntity::hide() {
	visible = false;
	SetSize(sf::Vector2f(0.0f,0.0f));
}