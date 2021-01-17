#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/SpriteCameraComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
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
			if (pixelColor.r == 0 && pixelColor.g == 0 && pixelColor.b == 255)//blue = interactive object
				board->CreateInteractiveObject(sf::Vector2i(x, y));
		}
	}
}

GameBoard::GameBoard()
	: m_player(nullptr)
	, m_gridSize(100.f)
	, m_treaSize(50.f)
	
{
	CreateBackground();
	CreateDarkScreen();
	LevelLoader::GetInstance()->LoadLevel(this);

	CreateMusic();
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

void GameBoard::CreateDarkScreen()
{
	GameEngine::Entity* darkScreen = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(darkScreen);
	
	darkScreen->SetPos(sf::Vector2f(850.f, 1000.f));
	darkScreen->SetSize(sf::Vector2f(1500.f, 1500.f));
	darkScreen->AddComponent<Game::SpriteCameraComponent>();
	darkScreen->AddComponent<Game::PlayerMovementComponent>();

	//Render
	GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>
		(darkScreen->AddComponent<GameEngine::SpriteRenderComponent>());

	render->SetFillColor(sf::Color::Transparent);
	render->SetTexture(GameEngine::eTexture::type::DarkScreen);
	render->SetZLevel(8);
	
	
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


void GameBoard::CreateInteractiveObject(sf::Vector2i coords)
{
	GameEngine::Entity* interactiveObject = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(interactiveObject);

	interactiveObject->SetEntityType(GameEngine::EEntityType::InteractiveObject);
	//Spawn position is calculated as coordinates on the grid, times grid size
	//(so that 0,0 is in 0px -> 10, 5 is in 500px, 250px)
	//+ half the grid size, since our objects are spawned in it's middle
	float spawnPosX = coords.x * m_treaSize + (m_treaSize / 2.f);
	float spawnPosY = coords.y * m_treaSize + (m_treaSize / 2.f);

	interactiveObject->SetPos(sf::Vector2f(spawnPosX, spawnPosY));
	interactiveObject->SetSize(sf::Vector2f(m_treaSize, m_treaSize));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(interactiveObject->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::InteractiveObject);

	interactiveObject->AddComponent<GameEngine::CollidableComponent>();
	
}


GameBoard::~GameBoard()
{

}


void GameBoard::Update()
{	
	
}