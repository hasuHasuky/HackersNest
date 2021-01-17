#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include <string>

namespace GameEngine
{
    class SpriteRenderComponent;
}

namespace Game
{
    class DialogBackgroundEntity : public GameEngine::Entity
    {
    public:
        DialogBackgroundEntity();
        virtual void Update() override;
        virtual void OnAddToWorld() override;
        void show();
        void hide();
    private:
        GameEngine::SpriteRenderComponent* spriteRenderer;
        bool visible;
    };
}