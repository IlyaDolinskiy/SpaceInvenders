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

  bool Intersection(ISpaceObjects const & object)
  {
    QVector2D m_max1 = createMax(m_position, m_size);
    QVector2D m_min1 = createMin(m_position, m_size);
    QVector2D m_max2 = createMax(object.m_position, object.m_size);
    QVector2D m_min2 = createMin(object.m_position, object.m_size);

    return (m_max1.y() >= m_min2.y()) && (m_min1.y() <= m_max2.y()) &&
      (m_max1.x() >= m_min2.x()) && (m_min1.x() <= m_max2.x());
  }

  virtual void Damage(double harm) 
  {
    m_healf -= harm;
  }

  float GetHealf() const { return m_healf; }

  virtual void Move(float elapsedSeconds) {}

  bool IsActive() { return isActive; }

  void SetActive(bool active) { isActive = active; }

protected:
  QVector2D createMax(QVector2D const & v, QSize const & s)
  {
    return QVector2D(v + QVector2D(s.width()/2, s.height()/2));
  }

  QVector2D createMin(QVector2D const & v, QSize const & s)
  {
    return QVector2D(v - QVector2D(s.width()/2, s.height()/2));
  }

  float m_healf = 100.0f;  
  QSize m_size = QSize(64, 64);
  QVector2D m_position = QVector2D(200, 200);
  bool isActive = true;
};

