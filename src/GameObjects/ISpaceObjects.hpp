#pragma once

#include <QSize>
#include <QVector2D>

#include "IGameObjects.hpp"

class ISpaceObjects : public IGameObjects
{
public:
  ~ISpaceObjects() override {};
  GameObjectsTypes GetType() const override = 0;

  QVector2D & GetPosition() { return m_position; }
  QVector2D const & GetPosition() const { return m_position; }
  QSize & GetSize() { return m_size; }
  QSize const & GetSize() const { return m_size; }

  void SetPosition(QVector2D const & position) { m_position = position; }
  void SetSize(QSize const & size) { m_size = size; }

protected:
  QSize m_size = QSize(64, 64);
  QVector2D m_position = QVector2D(200, 200);
};