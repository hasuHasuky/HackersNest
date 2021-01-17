#include <SFML/Window/Keyboard.hpp> //<-- Add the keyboard include in order to get keyboard inputs
#include "Game/Components/PlayerMovementComponent.h"
#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"

using namespace Game;
bool PlayerMovementComponent::game_paused = false;

void PlayerMovementComponent::Update()
{  
    Component::Update();
    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    int speed_difference = GameEngine::CollidablePhysicsComponent::speed_change;

    //By default the displacement is 0,0
    sf::Vector2f displacement{0.0f, 0.0f};

    //The amount of speed that we will apply when input is received

    const float inputAmount = 150.0f;
    if (!game_paused) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            displacement.x -= (inputAmount - speed_difference) * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            displacement.x += (inputAmount - speed_difference) * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            displacement.y -= (inputAmount - speed_difference) * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            displacement.y += (inputAmount - speed_difference) * dt;
        }
    }
    else {
        displacement.x = 0.0f;
        displacement.y = 0.0f;
    }


    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);
}

void PlayerMovementComponent::OnAddToWorld() {}