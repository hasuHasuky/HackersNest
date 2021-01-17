#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/SpriteCameraComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include <SFML/System/Vector2.hpp>

using namespace Game;

GameBoard::GameBoard()
{
    CreatePlayer();
	CreateBackground();
	CreateObstacle();
	CreateDarkScreen();
}

void GameBoard::CreatePlayer()
{
	m_player = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);

	m_player->SetPos(sf::Vector2f(50.0f, 50.0f));
	m_player->SetSize(sf::Vector2f(10.0f, 40.0f));

	//Render
	GameEngine::RenderComponent* render = m_player->AddComponent<GameEngine::RenderComponent>();

	render->SetFillColor(sf::Color::Red);

	//Movement
	m_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	m_player->AddComponent<Game::SpriteCameraComponent>();  // <-- Attach Camera to the player
	m_player->AddComponent<GameEngine::CollidablePhysicsComponent>(); // <-- For colliding with obstacle
}

void GameBoard::CreateBackground()
{
	GameEngine::Entity* background = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(background);

	background->SetPos(sf::Vector2f(200.f, 150.f));
	background->SetSize(sf::Vector2f(2300.f, 1900.f));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(background->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Background);
	spriteRender->SetZLevel(-1);
}

void GameBoard::CreateObstacle()
{
	GameEngine::Entity* obstacle = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(obstacle);

	obstacle->SetPos(sf::Vector2f(60.f, 60.f));
	obstacle->SetSize(sf::Vector2f(200.f, 200.f));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(obstacle->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Obstacle);

	obstacle->AddComponent<GameEngine::CollidableComponent>();
}

void GameBoard::CreateDarkScreen()
{
	GameEngine::Entity* darkScreen = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(darkScreen);

	darkScreen->SetPos(sf::Vector2f(0.f, 0.f));
	darkScreen->SetSize(sf::Vector2f(500.f, 500.f));

	//Render
	GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>
		(darkScreen->AddComponent<GameEngine::SpriteRenderComponent>());

	render->SetFillColor(sf::Color::Transparent);
	render->SetTexture(GameEngine::eTexture::type::DarkScreen);
	render->SetZLevel(8);

	//LinkedEntityComponent* linkedCmp = static_cast<LinkedEntityComponent*>
	//						(background->AddComponent<LinkedEntityComponent>());

	//linkedCmp->SetFollowedEntity(m_player);
	//inkedCmp->SetFollwedOffset(sf::Vector2f(0.f, -200.f));
}

GameBoard::~GameBoard()
{

}


void GameBoard::Update()
{	
	
}