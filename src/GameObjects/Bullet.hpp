#pragma once

#include "ISpaceObjects.hpp"

enum class BulletParent {User, Alien};

class Bullet : public ISpaceObjects
{
public:
  ~Bullet() override {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::Bullet; }

  float GetEnergy() const { return energy; }
  BulletParent const & GetParent() const { return m_parent; }

protected:
  BulletParent m_parent = BulletParent::Alien;
  float energy = 25.0f;
};