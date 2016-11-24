#pragma once

#include "ISpaceObjects.hpp"

class Bullet : public ISpaceObjects
{
public:
  ~Bullet() override {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::Bullet; }
};