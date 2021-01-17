#include "CollidablePhysicsComponent.h"
#include "GameEngine/Util/CollisionManager.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "Game/Util/DialogManager.h"
#include <SFML/Window/Keyboard.hpp> //<-- Add the keyboard include in order to get keyboard inputs
#include <vector>

using namespace GameEngine;
using namespace Game;

float CollidablePhysicsComponent::speed_change = 0.0f;

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

	for (int a = 0; a < collidables.size(); ++a)
	{
		CollidableComponent *colComponent = collidables[a];
		if (colComponent == this)
			continue;

		AABBRect intersection;
		AABBRect myBox = GetWorldAABB();
		AABBRect colideBox = colComponent->GetWorldAABB();
		if (myBox.intersects(colideBox, intersection))
		{
			GameEngine::Entity *collidedEntity = colComponent->GetEntity();
			if (collidedEntity->Entity::GetEntityType() != EEntityType::Treasure)
			{
				DialogManager::GetInstance()->openDialog("You have picked up a treasure! Press space to continue");
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
				{
					speed_change += 0.1f;
					DialogManager::GetInstance()->closeDialog();
				}

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
	}
}