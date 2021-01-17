#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs
#include "PlayerMovementComponent.h"
#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"

using namespace Game;

PlayerMovementComponent::PlayerMovementComponent()
    : m_lastWalkIndex(0)
{

}

PlayerMovementComponent::~PlayerMovementComponent() {}

//void PlayerMovementComponent::OnAddToWorld() {}

void PlayerMovementComponent::Update()
{
    Component::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };

    //The amount of speed that we will apply when input is received
    const float inputAmount = 100.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        displacement.x -= inputAmount * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        displacement.x += inputAmount * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        displacement.y -= inputAmount * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        displacement.y += inputAmount * dt;
    }

    
    int maxIndex = 4;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_lastWalkIndex++;
        if (m_lastWalkIndex >= maxIndex)
            m_lastWalkIndex = 0;

        GameEngine::SpriteRenderComponent* spriteRender = GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();
        if (spriteRender)
        {
            spriteRender->SetTileIndex(sf::Vector2i(m_lastWalkIndex, 0));
        }
    }

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

}


