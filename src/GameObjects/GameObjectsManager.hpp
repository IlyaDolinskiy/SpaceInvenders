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

typedef std::shared_ptr<ISpaceObjects> ObjectPtr;

class GameObjectsManager: protected patterns::Singleton <GameObjectsManager>
{
public:

  void AddBullet(BulletPtr bullet) { m_bullets.push_back(static_cast<BulletPtr>(bullet)); }
  void AddAlien(AlienPtr alien) { m_aliens.push_back(static_cast<AlienPtr>(alien)); }
  void AddObstacles(ObstaclesPtr obstacles) { m_obstacles.push_back(static_cast<ObstaclesPtr>(obstacles)); }

  std::list<BulletPtr> const & GetBulletList() const { return m_bullets; }
  std::list<AlienPtr> const & GetAlienList() const { return m_aliens; }
  std::list<ObstaclesPtr> const & GetObstaclesList() const { return m_obstacles; }

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