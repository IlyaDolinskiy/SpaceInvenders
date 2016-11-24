#pragma once

#include "../Patterns/Factory.hpp"
#include "GameObjectsTypes.hpp"
#include "../Config.hpp"

class IGameObjects;

class GameObjectsFactory : 
    protected patterns::Singleton <GameObjectsFactory>, 
    public patterns::factory::Factory<IGameObjects, GameObjectsTypes
#if DEF_GAME_FACTORY_POLICY == DEF_GAME_FACTORY_POLICY_THROW_EXCEPTION
    ,patterns::factory::policy::FactoryThrowExceptionErrorPolicy
#endif // DEF_GAME_FACTORY_POLICY
    >

{
public:

protected:
  friend class patterns::Singleton<GameObjectsFactory>;
};

#define GameFactory patterns::Singleton<GameObjectsFactory>::Instance()