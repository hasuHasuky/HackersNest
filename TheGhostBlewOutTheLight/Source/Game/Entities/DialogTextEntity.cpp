#include "DialogTextEntity.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/GameEngineMain.h"

#include <iostream>

using namespace Game;

DialogTextEntity::DialogTextEntity() : timer{ 0 }, visible{ false }{
	textRenderer = AddComponent<GameEngine::TextRenderComponent>();
	textRenderer->SetFont("arial.ttf");
	textRenderer->SetColor(sf::Color::White);
	textRenderer->SetZLevel(10);
}

void DialogTextEntity::OnAddToWorld() {

}
void DialogTextEntity::OnRemoveFromWorld() {

}
void DialogTextEntity::Update() {
	if (!visible) return;
	sf::Vector2f pos = GameEngine::CameraManager::GetInstance()->GetCameraView().getCenter();
	sf::Vector2u size = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow()->getSize();
	SetPos(sf::Vector2f(pos.x-size.x/2+100,pos.y+size.y/4));
	timer += GameEngine::GameEngineMain::GetTimeDelta();
	if (timer > 0.04f) {
		timer = 0;
		if (currentString != targetString) {
			currentString += targetString[currentString.length()];
			textRenderer->SetString(currentString);
		}
	}
}

void DialogTextEntity::finishString() {
	currentString = targetString;
	textRenderer->SetString(currentString);
}

void DialogTextEntity::show() {
	textRenderer->SetCharacterSizePixels(20);
	visible = true;
}

void DialogTextEntity::hide() {
	textRenderer->SetCharacterSizePixels(0);
	textRenderer->SetString("");
	visible = false;
}

bool DialogTextEntity::isCompleted() {
	return currentString == targetString;
}