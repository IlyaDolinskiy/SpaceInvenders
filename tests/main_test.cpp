#include "gtest/gtest.h"

#include "GameObjects/SpaceCrafts/PlayerCraft.hpp"

TEST(start, main_test)
{
  PlayerCraft player;
  player.do_smth();
  EXPECT_EQ(player.m_i, 2);
}