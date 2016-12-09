#pragma once

#include "GameObjectsTypes.hpp"

class IGameObjects
{
public:
  virtual ~IGameObjects() {};
  virtual GameObjectsTypes GetType() const = 0;
};