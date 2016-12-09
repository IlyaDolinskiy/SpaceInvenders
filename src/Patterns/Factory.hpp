#pragma once

#include <memory>

#include "noncopyable.hpp"
#include "FactoryPolicy.hpp"

namespace patterns
{
namespace factory
{

template <class Base>
class ICreator : private noncopyable
{
public:
  virtual ~ICreator() = default;
  virtual Base* create() const = 0;
};

template <class C, class Base>
class Creator : public ICreator<Base>
{
public:
  Creator() = default;

  virtual ~Creator() {}

  Base * create() const  { return new C(); }
};


template <class Base, class IdType , template <class, class> class FactoryErrorPolicy = policy::FactoryIgnoreErrorPolicy>
class Factory : public FactoryErrorPolicy<Base, IdType>, private noncopyable
{
protected:
  typedef ICreator<Base> FactoryCreator;
  typedef std::map<IdType, FactoryCreator*> FactoryMap;

public:

  virtual ~Factory()
  {
    for (typename FactoryMap::iterator it = m_map.begin(), endIt = m_map.end(); it != endIt; ++it)  
      delete it->second;
  }

  Base * Create(IdType const & id) const
  {
    auto it = m_map.find(id);
    if (it != m_map.end())
    {
      return it->second->create();
    }
    return FactoryErrorPolicy<Base, IdType>::OnCreateFailed(id);
  }

  template <typename C>
  void Add(IdType const & id)
  {
    RegisterClass(id, new Creator<C, Base>);
  }

  void Remove(IdType const & id)
  {
    auto it = m_map.find(id);
    if (it != m_map.end())
    {
      delete it->second;
      m_map.erase(it);
    }
    else
      FactoryErrorPolicy<Base, IdType>::OnRemoveFailed(id);
  }

  bool IsRegistered(IdType const & id) const
  {
    return m_map.find(id) != m_map.end();
  }

  size_t Size() const
  {
    return m_map.size();
  }
  
protected:
  void RegisterClass(IdType const & id, FactoryCreator * pFactory)
  {
    std::unique_ptr<FactoryCreator> ptr(pFactory);
    auto it = m_map.find(id);
    if (it == m_map.end())
    {
      m_map[id] = ptr.release();
    }
    else
      FactoryErrorPolicy<Base, IdType>::OnDuplicateRegistered(id);
  }

private:
  FactoryMap m_map;
};

} // namespace factory

} // namespace patterns