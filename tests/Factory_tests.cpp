#include "gtest/gtest.h"
#include "GameObjects/GameObjects.hpp"
#include "Patterns/Factory.hpp"


TEST(Factory, registration)
{
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), false);

  GameFactory.Add<Bullet>(GameObjectsTypes::Bullet);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), false);

  GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), false);

  GameFactory.Add<AlienCraft>(GameObjectsTypes::AlienCraft);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), true);


  GameFactory.Remove(GameObjectsTypes::Bullet);
  GameFactory.Remove(GameObjectsTypes::Obstacles);
  GameFactory.Remove(GameObjectsTypes::AlienCraft);
}

TEST(Factory, unregistration)
{

  GameFactory.Add<Bullet>(GameObjectsTypes::Bullet);
  GameFactory.Add<Bullet>(GameObjectsTypes::Obstacles);
  GameFactory.Add<Bullet>(GameObjectsTypes::AlienCraft);

  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), true);

  GameFactory.Remove(GameObjectsTypes::Bullet);


  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), true);

  GameFactory.Remove(GameObjectsTypes::Obstacles);


  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), true);

  GameFactory.Remove(GameObjectsTypes::AlienCraft);

  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), false);
}

TEST(Factory, create)
{
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), false);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), false);

  GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles);
  GameFactory.Add<Bullet>(GameObjectsTypes::Bullet);
  GameFactory.Add<AlienCraft>(GameObjectsTypes::AlienCraft);

  auto bullet = GameFactory.Create(GameObjectsTypes::Bullet);
  auto obstacles = GameFactory.Create(GameObjectsTypes::Obstacles);
  auto alienCraft = GameFactory.Create(GameObjectsTypes::AlienCraft);

  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Bullet), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::Obstacles), true);
  EXPECT_EQ(GameFactory.IsRegistered(GameObjectsTypes::AlienCraft), true);

  EXPECT_EQ(bullet->GetType(), GameObjectsTypes::Bullet);
  EXPECT_EQ(obstacles->GetType(), GameObjectsTypes::Obstacles);
  EXPECT_EQ(alienCraft->GetType(), GameObjectsTypes::AlienCraft);


  GameFactory.Remove(GameObjectsTypes::Obstacles);
  GameFactory.Remove(GameObjectsTypes::Bullet);
  GameFactory.Remove(GameObjectsTypes::AlienCraft);

  delete bullet;
  delete obstacles;
  delete alienCraft;
}

#if DEF_GAME_FACTORY_POLICY == DEF_GAME_FACTORY_POLICY_THROW_EXCEPTION
using FactoryException = patterns::factory::policy::FactoryException;
TEST(FactoryPolicy, test)
{
  GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles);
  ASSERT_THROW(GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles), FactoryException);
  ASSERT_THROW(GameFactory.Remove(GameObjectsTypes::AlienCraft), FactoryException);
  ASSERT_THROW(GameFactory.Create(GameObjectsTypes::AlienCraft), FactoryException);
}
#endif // DEF_GAME_FACTORY_POLICY