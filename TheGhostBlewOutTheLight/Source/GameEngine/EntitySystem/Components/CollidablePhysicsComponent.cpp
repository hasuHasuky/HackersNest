#include "CollidablePhysicsComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/Util/CollisionManager.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "Game/Util/DialogManager.h"
#include <SFML/Window/Keyboard.hpp> //<-- Add the keyboard include in order to get keyboard inputs
#include <vector>
#include "Game/Components/PlayerMovementComponent.h"
#include <iostream>

using namespace GameEngine;
using namespace Game;
static bool touched_treasure = false;
static bool touched_interactive_object = false;
static bool answered_question = false;
float CollidablePhysicsComponent::speed_change = 0.0f;
int CollidablePhysicsComponent::fortune = 0;
static bool started = false;
static float timer = 0;
static bool prev_space_pressed = false;
CollidablePhysicsComponent::CollidablePhysicsComponent()
{
}

CollidablePhysicsComponent::~CollidablePhysicsComponent()
{
}

void CollidablePhysicsComponent::OnAddToWorld()
{
	CollidableComponent::OnAddToWorld();
}

void CollidablePhysicsComponent::OnRemoveFromWorld()
{
	CollidableComponent::OnRemoveFromWorld();
}

void CollidablePhysicsComponent::Update()
{
	//For the time being just a simple intersection check that moves the entity out of all potential intersect boxes
	std::vector<CollidableComponent *> &collidables = CollisionManager::GetInstance()->GetCollidables();
	if (!started) {
		if (!PlayerMovementComponent::game_paused) {
			DialogManager::GetInstance()->openDialog("You have chose to enter an ancient emperor's tomb for its treasure.\nWalk around to observe.\nPress space to continue.");
			PlayerMovementComponent::game_paused = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!prev_space_pressed && DialogManager::GetInstance()->isDialogCompleted()) {
				started = true;
				DialogManager::GetInstance()->closeDialog();
   				PlayerMovementComponent::game_paused = false;
			}
			else {
				DialogManager::GetInstance()->finishDialog();
			}
		}
	}

	for (int a = 0; a < collidables.size(); ++a)
	{
		CollidableComponent *colComponent = collidables[a];
		if (colComponent == this)
			continue;

		AABBRect intersection;
		AABBRect myBox = GetWorldAABB();
		AABBRect colideBox = colComponent->GetWorldAABB();

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && answered_question)
		{
			if (DialogManager::GetInstance()->isDialogCompleted() && !prev_space_pressed) {
				DialogManager::GetInstance()->closeDialog();
				touched_treasure = false;
				touched_interactive_object = false;
				answered_question = false;
			}
			else {
				DialogManager::GetInstance()->finishDialog();
			}
		}

		if (!answered_question && touched_treasure)
		{
			bool just_pressed = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				DialogManager::GetInstance()->finishDialog();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
				fortune++;
				speed_change += 20.0f;
				DialogManager::GetInstance()->closeDialog();
				DialogManager::GetInstance()->openDialog("You have chosen to keep the treasure! Press space to close this text");
				PlayerMovementComponent::game_paused = false;
				answered_question = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			{
				DialogManager::GetInstance()->closeDialog();
				DialogManager::GetInstance()->openDialog("You have chosen not to pick up the treasure! Press space to close this text");
				PlayerMovementComponent::game_paused = false;
				answered_question = true;
			}
		}

		if (!answered_question && touched_interactive_object)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				DialogManager::GetInstance()->finishDialog();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			{
				DialogManager::GetInstance()->closeDialog();
				// Do something
				DialogManager::GetInstance()->openDialog("You have examined it. Press space to close this text");
				PlayerMovementComponent::game_paused = false;
				answered_question = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			{
				DialogManager::GetInstance()->closeDialog();
				// Do something
				DialogManager::GetInstance()->openDialog("You have chosen to leave it. Press space to close this text");
				PlayerMovementComponent::game_paused = false;
				answered_question = true;
			}
		}
		
		if (myBox.intersects(colideBox, intersection))
		{
			GameEngine::Entity *collidedEntity = colComponent->GetEntity();

			if (!touched_treasure && (collidedEntity->Entity::GetEntityType() == EEntityType::Treasure))
			{
				DialogManager::GetInstance()->openDialog("You have found treasure. Would you like to keep it?\n y : yes \n n : no");
				PlayerMovementComponent::game_paused = true;
				touched_treasure = true;
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(collidedEntity);
			}

			if (collidedEntity->Entity::GetEntityType() == EEntityType::InteractiveObject)
			{
				DialogManager::GetInstance()->openDialog("Do you wish to examine this object?\n y : yes \n n : no");
				PlayerMovementComponent::game_paused = true;
				touched_interactive_object = true;
			}

			sf::Vector2f pos = GetEntity()->GetPos();
			if (intersection.width < intersection.height)
			{
				if (myBox.left < colideBox.left)
					pos.x -= intersection.width;
				else
					pos.x += intersection.width;
			}
			else
			{
				if (myBox.top < colideBox.top)
					pos.y -= intersection.height;
				else
					pos.y += intersection.height;
			}

			GetEntity()->SetPos(pos);
		}
		prev_space_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	}
}