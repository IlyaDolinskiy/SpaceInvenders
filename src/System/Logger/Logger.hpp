#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "../../Patterns/Singleton.hpp"


class Logger : public Singleton <Logger>
{
public:
  static const std::string Debug;
  static const std::string Info;
  static const std::string Error;

  void log(std::string const & message, std::string const & logLevel)
  {
    PrintToFile(message, logLevel);
  }

  Logger & operator() (std::string const & logLevel)
  {
    m_logLevel = logLevel;
    return *this;
  }

  Logger & operator<<(std::string const & message) 
  {
    log(message, m_logLevel);
  }

  template <typename T>
  Logger & operator<<(T const & obj) 
  {
    std::ostringstream os;
    os << obj;
    log(os.str(), m_logLevel);
    return *this;
  }

  template<typename T, template<typename, typename...> class C, typename... Args>
  Logger & operator << (C<T, Args...> const & objs)
  {
    std::ostringstream os;
    os << "[collection] : ";
    for (auto const & obj : objs)
      os << obj << ", ";
    log(os.str(), m_logLevel);
    return *this;
  }
  
protected:
  std::ofstream m_outputStream;
  static const char* const m_fileName;
  std::string m_logLevel;

  void PrintToFile(std::string const & message, std::string const & logLevel)
  { 
    m_outputStream.open(m_fileName, std::ios_base::app);
    m_outputStream << "[" << logLevel << "] : " << message << std::endl; 
    m_outputStream.close();
  }

  friend class Singleton<Logger>;
};

#define Log Singleton<Logger>::Instance()