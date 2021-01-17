#include <SFML/Window/Keyboard.hpp> //<-- Add the keyboard include in order to get keyboard inputs
#include "Game/Components/PlayerMovementComponent.h"
#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"

using namespace Game;
bool PlayerMovementComponent::game_paused = false;

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

    int speed_difference = GameEngine::CollidablePhysicsComponent::speed_change;

    //By default the displacement is 0,0
    sf::Vector2f displacement{0.0f, 0.0f};

    //The amount of speed that we will apply when input is received

    const float inputAmount = 150.0f;
    GameEngine::EAnimationId::type animId = GameEngine::EAnimationId::type::None;
    if (!game_paused) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            displacement.x -= (inputAmount - speed_difference) * dt;
            animId = GameEngine::EAnimationId::type::PlayerWalkBack;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            displacement.x += (inputAmount - speed_difference) * dt;
            animId = GameEngine::EAnimationId::type::PlayerWalkBack;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            displacement.y -= (inputAmount - speed_difference) * dt;
            animId = GameEngine::EAnimationId::type::PlayerWalkBack;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            displacement.y += (inputAmount - speed_difference) * dt;
            animId = GameEngine::EAnimationId::type::PlayerWalkFront;
        }
    }
    else {
        displacement.x = 0.0f;
        displacement.y = 0.0f;
    }
    GameEngine::AnimationComponent* animComp = GetEntity()->GetComponent<GameEngine::AnimationComponent>();
    if (animId) {
        if (animComp && (!animComp->IsAnimPlaying()||animComp->GetCurrentAnimation() != animId)) {
            animComp->SetIsLooping(true);
            animComp->PlayAnim(animId);
        }
    }
    else {
        if (animComp && animComp->IsAnimPlaying()) {
            animComp->StopAnim();
        }
    }
    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

}


