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
			Player,
			InteractiveObject,
			DarkScreenRed,
			Count //MUST BE THE LAST
		};
	}	

	inline const char* GetPath(eTexture::type texture)
	{
		switch (texture)
		{
		case eTexture::type::DarkScreen: return "darkScreenTest.png";
		case eTexture::type::DarkScreenRed: return "darkScreen.png";
		case eTexture::type::Background: return "background.png";
		case eTexture::type::Obstacle: return "stone.jpg";
		case eTexture::type::Level: return "wall.png";
		case eTexture::type::Treasure: return "treasure.png";
		case eTexture::type::Player: return "walk.png";
		case eTexture::type::InteractiveObject: return "puppet.png"; //change
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
	static sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture) {
		switch (texture) {
		case GameEngine::eTexture::Player: return sf::Vector2f(200.f, 200.f);
		}
		return sf::Vector2f(-1.f, -1.f);
	}
}

