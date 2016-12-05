#pragma once

#include "ISpaceObjects.hpp"

enum class BulletParent {User, Alien};

class Bullet : public ISpaceObjects
{
public:
  ~Bullet() override {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::Bullet; }
protected:
  BulletParent m_parent = BulletParent::Alien;
};