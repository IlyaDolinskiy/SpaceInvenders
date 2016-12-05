#pragma once

#include <list>
#include <memory>



#include "GameObjects/GameObjects.hpp"

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

  void Intersections()
  {
    for (auto bullet: m_bullets)
    {
      if (bullet->GetParent() == BulletParent::User)
        for (auto alien: m_aliens)
        {
          if (bullet->Intersection(*(alien.get())))
          {
            // *10 temp
            alien->Damage(bullet->GetEnergy()*10.0f);
          }
        }
      else
      {
        // player check
        if (bullet->Intersection(Player))
        {
          Player.Damage(bullet->GetEnergy());
        }
      }
      for (auto obstacles: m_obstacles)
      {
        if (bullet->Intersection(*(obstacles.get())));
        {
          // *10 temp
          obstacles->Damage(bullet->GetEnergy()*10.0f);
        }
      }
    }
  }

protected:
  friend class patterns::Singleton<GameObjectsManager>;

  std::list<BulletPtr> m_bullets;
  std::list<AlienPtr> m_aliens;
  std::list<ObstaclesPtr> m_obstacles;
};

#define GameManager patterns::Singleton<GameObjectsManager>::Instance()