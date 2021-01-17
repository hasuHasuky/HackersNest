#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/SpriteCameraComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/GameEngineMain.h"
#include <string>
#include <SFML/System/Vector2.hpp>

using namespace Game;

LevelLoader* LevelLoader::sm_instance = nullptr;

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

void LevelLoader::LoadLevel(GameBoard* board)
{
	//Get the texture of the level from texture manager
	sf::Texture* level = GameEngine::TextureManager::GetInstance()->GetTexture(GameEngine::eTexture::Level);
	//Make an image from that texture - texture do not have access to pixel information, but an image does!
	sf::Image levelImage = level->copyToImage();
	
	//For all pixels in textuure
	for (unsigned int y = 0; y < levelImage.getSize().y; y++)
	{
		for (unsigned int x = 0; x < levelImage.getSize().x; x++)
		{
			sf::Color pixelColor = levelImage.getPixel(x, y);

			//If pixel is black -> create an obstacle in pixel coordinates
			if (pixelColor.r == 0 && pixelColor.g == 0 && pixelColor.b == 0)//black = wall
				board->CreateObstacle(sf::Vector2i(x, y));
			if (pixelColor.r == 255 && pixelColor.g == 235 && pixelColor.b == 59)//yellow = treasure
				board->CreateTreasure(sf::Vector2i(x, y));
			if (pixelColor.r == 255 && pixelColor.g == 0 && pixelColor.b == 0)//red = player
				board->CreatePlayer(sf::Vector2i(x, y));
		}
	}
}

GameBoard::GameBoard()
	: m_player(nullptr)
	, m_gridSize(100.f)
	, m_treaSize(50.f)
	, time(0.f)
	
{
	LevelLoader::GetInstance()->LoadLevel(this);

	CreateMusic();
	CreateFortuneBar();
}



void GameBoard::CreatePlayer(sf::Vector2i coords)
{
	m_player = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);
	m_player->SetEntityType(GameEngine::EEntityType::Player);

	float spawnPosX = coords.x * m_gridSize + (m_gridSize / 2.f);
	float spawnPosY = coords.y * m_gridSize + (m_gridSize / 2.f);

	m_player->SetPos(sf::Vector2f(spawnPosX, spawnPosY));
	m_player->SetSize(sf::Vector2f(100.0f, 100.0f));

	//Render
	GameEngine::RenderComponent* render = m_player->AddComponent<GameEngine::RenderComponent>();

	render->SetFillColor(sf::Color::Red);

	//Movement
	m_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	m_player->AddComponent<Game::SpriteCameraComponent>();  // <-- Attach Camera to the player
	m_player->AddComponent<GameEngine::CollidablePhysicsComponent>(); // <-- For colliding with obstacle
}


void GameBoard::CreateObstacle(sf::Vector2i coords)
{
	GameEngine::Entity* obstacle = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(obstacle);

	obstacle->SetEntityType(GameEngine::EEntityType::Obstacle);
	//Spawn position is calculated as coordinates on the grid, times grid size
	//(so that 0,0 is in 0px -> 10, 5 is in 500px, 250px)
	//+ half the grid size, since our objects are spawned in it's middle
	float spawnPosX = coords.x * m_gridSize + (m_gridSize / 2.f);
	float spawnPosY = coords.y * m_gridSize + (m_gridSize / 2.f);
	
	obstacle->SetPos(sf::Vector2f(spawnPosX, spawnPosY));
	obstacle->SetSize(sf::Vector2f(m_gridSize, m_gridSize));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(obstacle->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Obstacle);

	obstacle->AddComponent<GameEngine::CollidableComponent>();
}

void GameBoard::CreateMusic() {
	m_sound = m_player->AddComponent<GameEngine::SoundComponent>();
	m_sound->SetNumSimultaneousSounds(2);
	bgmID = m_sound->LoadSoundFromFile("Resources/sound/1.wav");
	m_sound->PlaySound(bgmID,false,true,10.f);
}

void GameBoard::CreateTreasure(sf::Vector2i coords)
{
	GameEngine::Entity* treasure = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(treasure);

	treasure->SetEntityType(GameEngine::EEntityType::Treasure);
	//Spawn position is calculated as coordinates on the grid, times grid size
	//(so that 0,0 is in 0px -> 10, 5 is in 500px, 250px)
	//+ half the grid size, since our objects are spawned in it's middle
	float spawnPosX = coords.x * m_treaSize + (m_treaSize / 2.f);
	float spawnPosY = coords.y * m_treaSize + (m_treaSize / 2.f);

	treasure->SetPos(sf::Vector2f(spawnPosX, spawnPosY));
	treasure->SetSize(sf::Vector2f(m_treaSize, m_treaSize));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(treasure->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Treasure);

	treasure->AddComponent<GameEngine::CollidableComponent>();
	//treasure->AddComponent<GameEngine::CollidablePhysicsComponent>();
}

void GameBoard::CreateFortuneBar() {
	m_fortune_bar = new GameEngine::Entity();
	m_fortune_text = new GameEngine::Entity();
	m_timer_text = new GameEngine::Entity();
	fortuneTextComponent = m_fortune_text->AddComponent<GameEngine::TextRenderComponent>();
	fortuneBarComponent = m_fortune_bar->AddComponent<GameEngine::RenderComponent>();
	timerTextComponent = m_timer_text->AddComponent<GameEngine::TextRenderComponent>();
	//Fortune
	fortuneTextComponent->SetColor(sf::Color::White);
	fortuneTextComponent->SetFont("arial.ttf");
	fortuneTextComponent->SetCharacterSizePixels(25);
	fortuneTextComponent->SetZLevel(11);
	fortuneBarComponent->SetFillColor(sf::Color::White);
	fortuneBarComponent->SetZLevel(11);
	//Timer
	timerTextComponent->SetColor(sf::Color::White);
	timerTextComponent->SetFont("arial.ttf");
	timerTextComponent->SetCharacterSizePixels(25);
	timerTextComponent->SetZLevel(11);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_fortune_text);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_fortune_bar);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_timer_text);
}

GameBoard::~GameBoard()
{

}

void GameBoard::Update()
{
	int fortune = GameEngine::CollidablePhysicsComponent::fortune;
	sf::Vector2f pos = GameEngine::CameraManager::GetInstance()->GetCameraView().getCenter();
	sf::Vector2u size = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow()->getSize();
	int width = fortune * 10.f;
	m_fortune_text->SetPos(sf::Vector2f(pos.x - size.x / 2 + 30, pos.y - size.y / 2 + 25));
	m_fortune_bar->SetPos(sf::Vector2f(pos.x - size.x / 2 + 30 + width/2, pos.y - size.y / 2 + 75));
	m_timer_text->SetPos(sf::Vector2f(pos.x + size.x / 2 - 150, pos.y - size.y / 2 + 25));
	fortuneTextComponent->SetString("Fortune: " + std::to_string(fortune*100));
	m_fortune_bar->SetSize(sf::Vector2f(width, 30.f));
	timerTextComponent->SetString("Time: " + std::to_string((int) GameEngine::GameEngineMain::GetInstance()->GetGameTime()) + "s");
}