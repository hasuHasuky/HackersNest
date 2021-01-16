#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "Components/PlayerMovementComponent.h"
#include "Components/SpriteCameraComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
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
			if (pixelColor.r == 0 && pixelColor.g == 0 && pixelColor.b == 0)//detecting black color
				board->CreateObstacle(sf::Vector2i(x, y));
		}
	}
}

GameBoard::GameBoard()
	: m_player(nullptr)
	, m_gridSize(50.f)
	
{
	LevelLoader::GetInstance()->LoadLevel(this);

    CreatePlayer();
	CreateObstacle();
}



void GameBoard::CreatePlayer()
{
	m_player = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);

	m_player->SetPos(sf::Vector2f(50.0f, 50.0f));
	m_player->SetSize(sf::Vector2f(25.0f, 25.0f));

	//Render
	GameEngine::RenderComponent* render = m_player->AddComponent<GameEngine::RenderComponent>();

	render->SetFillColor(sf::Color::Red);

	//Movement
	m_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	//m_player->AddComponent<Game::SpriteCameraComponent>();  // <-- Attach Camera to the player
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


GameBoard::~GameBoard()
{

}


void GameBoard::Update()
{	
	
}