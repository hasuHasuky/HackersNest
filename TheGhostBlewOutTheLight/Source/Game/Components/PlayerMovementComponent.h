#pragma once

#include "GameEngine/EntitySystem/Component.h"

namespace Game
{
    class PlayerMovementComponent : public GameEngine::Component
    {
    public:
        static bool game_paused;
        virtual void Update() override;
        PlayerMovementComponent();
        ~PlayerMovementComponent();
        int m_lastWalkIndex;
    };
}