#pragma once

#include <list>
#include <memory>

#include "SpaceCrafts/SpaceCrafts.hpp"
#include "Bullet.hpp"
#include "Obstacles.hpp"
#include "../Patterns/Singleton.hpp"
#include <random>
#include <algorithm>
#include <chrono> 
#include "Settings.hpp"

typedef std::shared_ptr<Bullet> BulletPtr;
typedef std::shared_ptr<AlienCraft> AlienPtr;
typedef std::shared_ptr<Obstacles> ObstaclesPtr;

typedef std::shared_ptr<ISpaceObjects> ObjectPtr;

class Math {
public:
  static int const GetRandomInt(int x, int y) {
    std::random_device rseed;
    std::mt19937 rgen(rseed());
    return static_cast<int>(std::uniform_int_distribution<int>(x,y)(rgen)); 
  }
protected:

};

class GameObjectsManager: protected patterns::Singleton <GameObjectsManager>
{
public:

  void AddBullet(BulletPtr bullet) { m_bullets.push_back(static_cast<BulletPtr>(bullet)); }
  void AddAlien(AlienPtr alien) { m_aliens.push_back(static_cast<AlienPtr>(alien)); }
  void AddObstacle(ObstaclesPtr obstacles) { m_obstacles.push_back(static_cast<ObstaclesPtr>(obstacles)); }

  std::list<BulletPtr> const & GetBulletList() const { return m_bullets; }
  std::list<AlienPtr> const & GetAlienList() const { return m_aliens; }
  std::list<ObstaclesPtr> const & GetObstaclesList() const { return m_obstacles; }

  void AlienShot()
  {
    if ((m_aliens.size() <= 0) || !m_isPlaying) return;
    try
    {
      auto it = m_aliens.begin();
      std::advance(it, Math::GetRandomInt(0, m_aliens.size()-1));


      QVector2D pos =  it->get()->GetPosition();

      auto bulletObj = GameFactory.Create(GameObjectsTypes::Bullet);
      bulletObj->SetPosition(pos);
      auto bullet = static_cast<Bullet*>(bulletObj);
      bullet->SetDirection(Player.GetPosition() - pos);
      bullet->SetParent(BulletParent::Alien);
      AddBullet(std::shared_ptr<Bullet>(bullet));
    }
    catch(...) {}
  }

  void Intersections()
  {
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
          [](BulletPtr element) -> bool {
              return !element->IsActive();
          }
      ), m_bullets.end()
    );
    m_aliens.erase(std::remove_if(m_aliens.begin(), m_aliens.end(),
          [](AlienPtr element) -> bool {
              return !element->IsActive();
          }
      ), m_aliens.end()
    );
    m_obstacles.erase(std::remove_if(m_obstacles.begin(), m_obstacles.end(),
          [](ObstaclesPtr element) -> bool {
              return !element->IsActive();
          }
      ), m_obstacles.end()
    );
    if (m_aliens.size() == 0)
      m_isPlaying = false;

    for (auto const & bullet: m_bullets)
    {
      if (bullet->IsActive() || m_isPlaying)
      {
        if (bullet->GetParent() == BulletParent::User)
          for (auto const & alien : m_aliens)
          {
            if (bullet->Intersection(*(alien.get())))
            {
              // *10 temp
              alien->Damage(bullet->GetEnergy());
              bullet->SetActive(false);
              if (alien->GetHealf() <= 0)
                alien->SetActive(false);
              continue;
            }
          }
        else
        {
          // player check
          if (bullet->Intersection(Player))
          {
            Player.Damage(bullet->GetEnergy());
            bullet->SetActive(false);
            std::cout << "Player Damage " << std::endl;
            if (Player.GetHealf() <= 0)
            {
              m_isPlaying = false;
              std::cout << "Game End!" << std::endl;
            }
            continue;
          }
        }
        for (auto const & obstacles: m_obstacles)
        {
          if (bullet->Intersection(*(obstacles.get())));
          {
            obstacles->Damage(bullet->GetEnergy()/10.0);
            //bullet->SetActive(false);

            if (obstacles->GetHealf() <= 0)
              obstacles->SetActive(false);

            continue;
          }
        }
      }
    }
  }

  void SetGameStatus(bool s) { m_isPlaying = s; }
  bool const GetGameStatus() const { return m_isPlaying; }

  void Restart()
  {
    m_aliens.clear();

    Player.SetHealf(300);
    Player.SetPosition(QVector2D(Settings.GetWindowQSize().width()/2, 20));

    int alienCount = Settings.Get()["AlienCount"].asInt();
    int w = Settings.GetWindowQSize().width() - 200;
    int h = Settings.GetWindowQSize().height() - 200;
    
    w /= 8;
    h /= 6;

    int feel[8][6];

    memset(feel, 0, 8*6*sizeof(int));

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist6(0, 5);
    std::uniform_int_distribution<int> dist8(0, 7);

    while (alienCount > 0)
    {
      int one = dist8(mt);
      int two = dist6(mt);
      if (feel[one][two] != 1)
      {
        feel[one][two] = 1;
        alienCount--;
      }
    }
    for (int i = 0; i < 8; ++i)
      for (int k = 0; k < 6; ++k)
      {
        if (feel[i][k] == 1)
        {
          auto alien = GameFactory.Create(GameObjectsTypes::AlienCraft);
          alien->SetPosition(QVector2D(100+w*i, h*k + 200 ));
          AddAlien(std::shared_ptr<AlienCraft>(static_cast<AlienCraft*>(alien)));
        }
      }
    /*for (int i = 0; i < 8; ++i)
    {
      auto obstacle = GameFactory.Create(GameObjectsTypes::Obstacles);
      obstacle->SetPosition(QVector2D(100+(w)*i, 150 ));
      GameManager.AddObstacle(std::shared_ptr<Obstacles>(static_cast<Obstacles*>(obstacle)));
    }*/
    m_timing = 3;
    m_isPlaying = true;
  }

  int GetTiming() {return m_timing;}
  void SetTiming(int t) {m_timing = t;}
  int PopTiming() 
  { 
    if (m_timing == 0)
      m_isPlaying = true;
    else
      m_timing--;
    return m_timing;
  }

protected:
  friend class patterns::Singleton<GameObjectsManager>;

  std::list<BulletPtr> m_bullets;
  std::list<AlienPtr> m_aliens;
  std::list<ObstaclesPtr> m_obstacles;

  int m_timing = 3;
  bool m_isPlaying = true;
};

#define GameManager patterns::Singleton<GameObjectsManager>::Instance()