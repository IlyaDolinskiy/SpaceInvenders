#pragma once

#include <string>
#include <fstream>

#include "json/json.h"
#include "Patterns/Singleton.hpp"

class GameSettings: protected patterns::Singleton <GameSettings>
{
public:

  void Load()
  {
    std::ifstream file("settings.json");
    if (file.is_open())
    {
      file >> m_settings;
      file.close();
    }
  }

  void Save()
  {
    //auto & root = m_settings["settings"];
    std::ofstream settingsFile("settings.json");
    if (settingsFile.is_open())
    {
      Json::StyledWriter styledWriter;
      settingsFile << styledWriter.write(m_settings);
      settingsFile.close();
    }
  }

  Json::Value const & Get() const { return m_settings["settings"]; }

  QSize GetWindowQSize() const 
  {
    auto & size = m_settings["settings"]["WindowSize"];
    return QSize(size["width"].asInt(), size["height"].asInt());
  }

protected:
  Json::Value m_settings;
  friend class patterns::Singleton<GameSettings>;
};

#define Settings patterns::Singleton<GameSettings>::Instance()