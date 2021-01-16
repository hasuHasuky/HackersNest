#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include <string>

namespace GameEngine
{
	class TextRenderComponent;
}

namespace Game {
	class DialogTextEntity : public GameEngine::Entity
	{
		public:
			DialogTextEntity();
			virtual void OnAddToWorld() override;
			virtual void OnRemoveFromWorld() override;
			virtual void Update() override;
			void setTargetString(std::string target) { currentString = ""; targetString = target; timer = 0; }
			void show();
			void hide();
			bool isCompleted();

		private:
			GameEngine::TextRenderComponent* textRenderer;
			std::string targetString;
			std::string currentString;
			float timer;
			bool visible;
	};
}