#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace patterns
{
namespace factory
{
namespace policy
{

class FactoryException : public std::exception
{
  std::string _msg;
public:
  FactoryException(std::string const & msg) throw() : _msg(msg) {}

  virtual ~FactoryException() throw() {}

  virtual char const * what() const throw() { return _msg.c_str(); }
};

template <class Base, class Type>
class FactoryIgnoreErrorPolicy
{
public:
  Base * OnCreateFailed(Type const & type) const { return nullptr; }

  void OnRemoveFailed(Type const & type) {}

  void OnDuplicateRegistered(Type const & type) {}
};

template <class Base, class Type>
class FactoryThrowExceptionErrorPolicy
{
public:
  std::string generateMessage(char const * msg, Type const & type) const
  {
    std::stringstream strm;
    std::string str = std::string(msg) +  std::string(", requested type id : ") + std::to_string(static_cast<int>(type));
    //strm << msg << ", requested type id : " << type;
    return strm.str();
  }

  Base * OnCreateFailed(Type const & type) const
  {
    throw FactoryException(generateMessage("Factory - can't create object (not registered)", type));
  }

  void OnRemoveFailed(Type const & type)
  {
    throw FactoryException(generateMessage("Factory - can't remove class (not registered)", type));
  }
  
  void OnDuplicateRegistered(Type const & type)
  {
    throw FactoryException(generateMessage("Factory - class already registered", type));
  }
};

} // namespace policy
template <class Base, class Type>
using FactoryIgnoreErrorPolicy = policy::FactoryIgnoreErrorPolicy<Base, Type>;

template <class Base, class Type>
using FactoryThrowExceptionErrorPolicy = policy::FactoryThrowExceptionErrorPolicy<Base, Type>;
} // namespace factory
} // namespace patterns