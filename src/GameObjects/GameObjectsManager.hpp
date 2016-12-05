#pragma once

#include <list>
#include <memory>

#include "SpaceCrafts/SpaceCrafts.hpp"
#include "Bullet.hpp"
#include "Obstacles.hpp"
#include "../Patterns/Singleton.hpp"

typedef std::shared_ptr<Bullet> BulletPtr;
typedef std::shared_ptr<AlienCraft> AlienPtr;
typedef std::shared_ptr<Obstacles> ObstaclesPtr;

class GameObjectsManager: protected patterns::Singleton <GameObjectsManager>
{
public:

  void AddBullet(BulletPtr bullet) { m_bullets.push_back(bullet); }
  void AddAlien(AlienPtr alien) { m_aliens.push_back(alien); }
  void AddObstacles(ObstaclesPtr obstacles) { m_obstacles.push_back(obstacles); }

  void Intersection()
  {
    for (auto bullet: m_bullets)
    {
      for (auto alien: m_aliens)
      {

      }
      for (auto obstacles: m_obstacles)
      {

      }
      // player check
    }
  }

protected:
  friend class patterns::Singleton<GameObjectsManager>;

  std::list<BulletPtr> m_bullets;
  std::list<AlienPtr> m_aliens;
  std::list<ObstaclesPtr> m_obstacles;
};

#define GameManager patterns::Singleton<GameObjectsManager>::Instance()