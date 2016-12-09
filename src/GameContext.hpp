#pragma once

#include <memory>

#include "Patterns/Singleton.hpp"

enum class GameStrategyType : unsigned char {Around};

class GameStrategy 
{
public:
  virtual ~GameStrategy() {}
  virtual void Use(void) = 0;
};

class Around: public GameStrategy
{
public:
  void Use(void) override {}
};


class GameContext: protected patterns::Singleton<GameContext>
{
public:
  void UseStrategy() { m_strategy->Use(); }

  void SetStrategy(GameStrategyType type) 
  { 
    m_type = type;
    switch(type)
    {
      case GameStrategyType::Around: m_strategy.reset(new Around);
        break;
    };
  }

  GameStrategyType const & GetStrategy() const noexcept { return m_type; }

protected:
  std::unique_ptr<GameStrategy> m_strategy;
  GameStrategyType m_type;
  friend class patterns::Singleton<GameContext>;
};

#define Context patterns::Singleton<GameContext>::Instance()