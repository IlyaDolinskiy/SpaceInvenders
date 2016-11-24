#include "gtest/gtest.h"

#include "GameObjects/GameObjects.hpp"

TEST(startA, main_testA)
{
  PlayerCraft player;
  player.do_smth();
  EXPECT_EQ(player.m_i, 2);
}