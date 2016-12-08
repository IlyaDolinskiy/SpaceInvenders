#pragma once

#include "ISpaceObjects.hpp"
#include <cmath>

#include <iostream>

enum class BulletParent {User, Alien};

class Bullet : public ISpaceObjects
{
public:
  Bullet() { m_size = QSize(16, 16);}
  
  ~Bullet() override {};

  GameObjectsTypes GetType() const override { return GameObjectsTypes::Bullet; }

  float GetEnergy() const { return m_energy; }

  BulletParent const & GetParent() const { return m_parent; }

  void SetParent(BulletParent const & parent) { m_parent = parent; }

  void SetDirection(QVector2D const & direction) 
  { 
    m_direction = direction; 
    m_direction.normalize();
  }

  void Move(float elapsedSeconds) override
  {
    m_position += m_direction*m_speed*elapsedSeconds;
  }
protected:
  QVector2D m_direction;
  BulletParent m_parent = BulletParent::Alien;
  float m_energy = 25.0f;
  float m_speed = 10.0f;
};