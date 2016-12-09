#include "gl_widget.hpp"

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QGuiApplication>
#include <cmath>

#include <iostream>


#include "GameObjects/GameObjects.hpp"
#include "GameObjects/GameObjectsManager.hpp"

#include "Settings.hpp"

#include <random>

#include "main_window.hpp"

namespace
{

int constexpr kLeftDirection = 0;
int constexpr kRightDirection = 1;
int constexpr kUpDirection = 2;
int constexpr kDownDirection = 3;

bool IsLeftButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::LeftButton;
}
bool IsLeftButton(QMouseEvent const * const e)
{
  return IsLeftButton(e->button()) || IsLeftButton(e->buttons());
}

bool IsRightButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::RightButton;
}
bool IsRightButton(QMouseEvent const * const e)
{
  return IsRightButton(e->button()) || IsRightButton(e->buttons());
}

} // namespace

GLWidget::GLWidget(MainWindow * mw, QColor const & background)
  : m_mainWindow(mw)
  , m_background(background)
{
  Settings.Load();

  setMinimumSize( Settings.GetWindowQSize() );
  setMaximumSize( Settings.GetWindowQSize() );
  setFocusPolicy(Qt::StrongFocus);

  GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles);
  GameFactory.Add<Bullet>(GameObjectsTypes::Bullet);
  GameFactory.Add<AlienCraft>(GameObjectsTypes::AlienCraft);

//
  GameManager.Restart();
}

GLWidget::~GLWidget()
{
  makeCurrent();
  delete m_textureAlien;
  delete m_textureBullet;
  delete m_textureGun;
  m_textureObstacle;
  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  m_textureAlien = new QOpenGLTexture(QImage(QString::fromUtf8(Settings.Get()["Sys"]["texture"]["alien"].asString().c_str())));
  m_textureBullet = new QOpenGLTexture(QImage(QString::fromUtf8(Settings.Get()["Sys"]["texture"]["bullet"].asString().c_str())));
  m_textureGun = new QOpenGLTexture(QImage(QString::fromUtf8(Settings.Get()["Sys"]["texture"]["gun"].asString().c_str())));
  m_textureObstacle = new QOpenGLTexture(QImage(QString::fromUtf8(Settings.Get()["Sys"]["texture"]["obstacle"].asString().c_str())));
 

  m_time.start();
}

void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();
  Update(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Render();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  painter.endNativePainting();

  if (elapsed != 0)
  {
    QString framesPerSecond;
    framesPerSecond.setNum(m_frames / (elapsed / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");
    painter.drawText(20, 55, QString::number(GameManager.GetBulletList().size()) + " bullets");
    painter.drawText(20, 70, QString::number(GameManager.GetAlienList().size()) + " aliens");
    //painter.drawText(20, 85, QString::number(GameManager.GetObstaclesList().size()) + " obstacles");

    if (!GameManager.GetGameStatus())
    {
      {
        painter.drawText(Player.GetPosition().x(), m_screenSize.height() - Player.GetPosition().y()-32, "The Game End!");
        
        int sp = std::accumulate(GameManager.GetAlienList().begin(), GameManager.GetAlienList().end(), 0, [](int s, AlienPtr const & alien) {
          return s + alien->GetHealf();
        });
        sp = 100*Settings.Get()["AlienCount"].asInt() - sp;
        painter.drawText(m_screenSize.width()/2-50, m_screenSize.height()/2, "you score: " + QString::number(sp));
        painter.drawText(m_screenSize.width()/2-50,  m_screenSize.height()/2+15, "Press f1 to restart");
        painter.drawText(m_screenSize.width()/2-50, m_screenSize.height()/2+30, "you HP: " + QString::number((int)Player.GetHealf()));
      }
    }
    else
    {
      painter.drawText(Player.GetPosition().x(), m_screenSize.height() - Player.GetPosition().y()-32, 
          QString::number((int)Player.GetHealf()) + " HP");
    }

    for (auto const & i: GameManager.GetAlienList())
      painter.drawText(i->GetPosition().x(), m_screenSize.height() - i->GetPosition().y()-32, 
          QString::number((int)i->GetHealf()) + " HP");
    for (auto const & i: GameManager.GetObstaclesList())
      painter.drawText(i->GetPosition().x(), m_screenSize.height() - i->GetPosition().y()-32, 
          QString::number((int)i->GetHealf()) + " HP");



  }
  painter.end();

  if (!(m_frames % 100))
  {
    m_time.start();
    m_frames = 0;
  }
  ++m_frames;
  update();
}

void GLWidget::resizeGL(int w, int h)
{
  m_screenSize.setWidth(w);
  m_screenSize.setHeight(h);
}

void GLWidget::EverySecond()
{

}


void GLWidget::Update(float elapsedSeconds)
{
  m_timeSecond += elapsedSeconds;
  // 100 = 1 sec
  if (m_timeSecond > 100.0) EverySecond();
  if (m_timeSecond > 10.0) GameManager.AlienShot();
  if (m_timeSecond > 1.0) GameManager.Intersections();
  while (m_timeSecond > 10.0) m_timeSecond -= 10.0;

  float const kSpeed = 20.0f; // pixels per second.

  auto position = Player.GetPosition();

  /*if (m_directions[kUpDirection])
    position.setY(position.y() + kSpeed * elapsedSeconds);
  if (m_directions[kDownDirection])
    position.setY(position.y() - kSpeed * elapsedSeconds);*/
  if ((m_directions[kLeftDirection]) && (position.x() - Player.GetSize().width()/2 > 0))
    position.setX(position.x() - kSpeed * elapsedSeconds);
  else
    if (m_directions[kLeftDirection])
      position.setX(Player.GetSize().width()/2);
  if ((m_directions[kRightDirection]) && (position.x() + Player.GetSize().width()/2 < m_screenSize.width()))
    position.setX(position.x() + kSpeed * elapsedSeconds);
  else
    if (m_directions[kRightDirection])
    position.setX(m_screenSize.width() - Player.GetSize().width()/2);

  for (auto const & i: GameManager.GetBulletList())
  {
    i->Move(elapsedSeconds);
  }

  Player.SetPosition(position);
}

void GLWidget::Render()
{
  m_texturedRect->Render(m_textureGun, Player.GetPosition(), Player.GetSize(), m_screenSize);

  for (auto const & i: GameManager.GetAlienList())
    m_texturedRect->Render(m_textureAlien, i->GetPosition(), i->GetSize(), m_screenSize);
  for (auto const & i: GameManager.GetObstaclesList())
    m_texturedRect->Render(m_textureObstacle, i->GetPosition(), i->GetSize(), m_screenSize);

  for (auto const & i: GameManager.GetBulletList())
  {
    if (i->GetPosition().x() < 0 || i->GetPosition().x() > m_screenSize.width() ||
        i->GetPosition().y() < 0 || i->GetPosition().y() > m_screenSize.height())
      i->SetActive(false);
    m_texturedRect->Render(m_textureBullet, i->GetPosition(), i->GetSize(), m_screenSize);
  }
}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QOpenGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = m_screenSize.height()-L2D(e->y());
  if (IsLeftButton(e) && GameManager.GetGameStatus())
  {
    auto bulletObj = GameFactory.Create(GameObjectsTypes::Bullet);
    bulletObj->SetPosition(Player.GetPosition());
    auto bullet = static_cast<Bullet*>(bulletObj);
    bullet->SetDirection(QVector2D(px, py) - Player.GetPosition());
    bullet->SetParent(BulletParent::User);
    GameManager.AddBullet(std::shared_ptr<Bullet>(bullet));
  }
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseDoubleClickEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsRightButton(e))
  {
    // ...
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseMoveEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseReleaseEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::wheelEvent(QWheelEvent * e)
{
  QOpenGLWidget::wheelEvent(e);

  int const delta = e->delta();
  int const px = L2D(e->x());
  int const py = L2D(e->y());
  // ...
}

void GLWidget::keyPressEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = true;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = true;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = true;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = false;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = false;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = false;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = false;
  else if (e->key() == Qt::Key_F1)
    GameManager.Restart();
}
