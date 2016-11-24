#include "gtest/gtest.h"

#include "GameObjects/GameObjects.hpp"

TEST(startA, main_testA)
{
  Player.do_smth();
  EXPECT_EQ(Player.m_i, 2);
}