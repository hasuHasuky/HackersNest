#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/Util/SoundManager.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace GameEngine {
	class SoundComponent;
}

namespace Game
{
	//Used for storing and controlling all game related entities, as well as providing an entry point for the "game" side of application	
	class PlayerEntity;	

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();

		void Update();
		bool IsGameOver() { return false; }

		void CreatePlayer(sf::Vector2i coords);
		void CreateObstacle(sf::Vector2i coords);
		void CreateTreasure(sf::Vector2i coords);

	private:
		GameEngine::Entity* m_player; // <-- Added Member
		float m_gridSize;
		float m_treaSize;
		void CreateMusic();
		GameEngine::SoundComponent* m_sound;
		GameEngine::SoundManager::SoundId bgmID;
	};

	class LevelLoader
	{
	public:
		LevelLoader();
		~LevelLoader();
		//SINGLETON IMPLEMENTATION -> allows us to use LevelLoader::GetInstance without pointer to an object
		//This is ia very standard pattern, when dealing with Managers and objects that should only exist ONCE
		//in our grame, and not have multiple instances.
		static LevelLoader* GetInstance() 
		{
			if (!sm_instance)
				sm_instance = new LevelLoader();
			return sm_instance;
		}

		void LoadLevel(GameBoard* board);

	private:
		static LevelLoader* sm_instance;
		void CreatePlayer();          // <-- Added Function
		void CreateObstacle();

	};
}

