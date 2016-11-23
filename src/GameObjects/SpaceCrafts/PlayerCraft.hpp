#pragma once

#include "SpaceCrafts.hpp"

class PlayerCraft : public SpaceCrafts
{
public:
  ~PlayerCraft() {};

  void do_smth();
public:
  int m_i;
};