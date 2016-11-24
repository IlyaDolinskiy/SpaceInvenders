#pragma once

#include "IGameObjects.hpp"

class ISpaceObjects : public IGameObjects
{
public:
  ~ISpaceObjects() override {};
  GameObjectsTypes GetType() const override = 0;
};