#pragma once

#include "../Patterns/Factory.hpp"
#include "GameObjectsTypes.hpp"

class IGameObjects;

class GameObjectsFactory : protected patterns::Singleton <GameObjectsFactory>, public patterns::factory::Factory<IGameObjects, GameObjectsTypes>
{
public:

protected:
  friend class patterns::Singleton<GameObjectsFactory>;
};

#define GameFactory patterns::Singleton<GameObjectsFactory>::Instance()