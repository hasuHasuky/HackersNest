#pragma once
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/Util/SoundManager.h"
#include <memory>

namespace GameEngine
{
	class SoundComponent : public Component
	{
	public:
		SoundComponent();
		virtual ~SoundComponent();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

		void SetNumSimultaneousSounds(unsigned int const numSimultaneousSounds);
		SoundManager::SoundId LoadSoundFromFile(std::string const& filename);
		void PlaySound(SoundManager::SoundId const soundIdToPlay, bool useExistingSoundInstance = false,bool loop = false,float volume=100);

	private:
		std::unique_ptr<SoundManager> m_soundManager;
	};
}

