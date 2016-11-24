#pragma once

#include "ISpaceObjects.hpp"

class Obstacles : public ISpaceObjects
{
public:
  virtual ~Obstacles() {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::Obstacles; }
};