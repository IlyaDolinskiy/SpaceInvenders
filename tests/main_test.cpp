#include "gtest/gtest.h"

#include <string>

#include "GameObjects/GameObjects.hpp"
#include "GameContext.hpp"
#include "System/Logger/Logger.hpp"
#include "Patterns/Factory.hpp"

TEST(startA, main_testA)
{
  Player.do_smth();
  EXPECT_EQ(Player.m_i, 2);
  Log(Logger::Info) << std::string("msg");

  Context.SetStrategy(GameStrategyType::Around);
  EXPECT_EQ(Context.GetStrategy(), GameStrategyType::Around);
}
