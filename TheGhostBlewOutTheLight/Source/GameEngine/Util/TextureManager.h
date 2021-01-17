#pragma once
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace GameEngine
{
	//TODO - if needed, move out of Engine part to some sort of loader on the game side of things
	namespace eTexture
	{
		enum type
		{
			None = -1,
			Obstacle,
			Background,
			DarkScreen, //For the dark screen on top
			Treasure,
			Level,
			Count,
		};
	}	

	inline const char* GetPath(eTexture::type texture)
	{
		switch (texture)
		{
		case eTexture::type::DarkScreen: return "darkScreenTest.png";
		case eTexture::type::Background: return "background.png";
		case eTexture::type::Obstacle: return "stone.jpg";
		case eTexture::type::Level: return "wall.png";
		case eTexture::type::Treasure: return "treasure.jpg";
		}
		return "UnknownTexType";
	}

	class TextureManager
	{
	public:
		static TextureManager* GetInstance() { if (!sm_instance) sm_instance = new TextureManager(); return sm_instance; }
		~TextureManager();

		void LoadTextures();
		void UnLoadTextures();

		sf::Texture* GetTexture(eTexture::type texture) const { return m_textures[(int)texture]; }

	private:
		TextureManager();
		static TextureManager* sm_instance;

		sf::Texture* m_textures[eTexture::Count];
	};
}

namespace TextureHelper
{
	sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture);
}

