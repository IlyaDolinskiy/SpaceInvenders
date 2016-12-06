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
  setMinimumSize(800, 600);
  setFocusPolicy(Qt::StrongFocus);

  GameFactory.Add<Obstacles>(GameObjectsTypes::Obstacles);
  GameFactory.Add<Bullet>(GameObjectsTypes::Bullet);
  GameFactory.Add<AlienCraft>(GameObjectsTypes::AlienCraft);

//
  for (int i = 0; i < 5; i++)
  {
    auto alien = GameFactory.Create(GameObjectsTypes::AlienCraft);
    alien->SetPosition(QVector2D(800/2 - 800/10*2.5 + 800/10*i, 400));
    GameManager.AddAlien(std::shared_ptr<AlienCraft>(static_cast<AlienCraft*>(alien)));

  }

}

GLWidget::~GLWidget()
{
  makeCurrent();
  delete m_textureAlien;
  delete m_textureBullet;
  delete m_textureGun;
  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  m_textureAlien = new QOpenGLTexture(QImage("data/alien.png"));
  m_textureBullet = new QOpenGLTexture(QImage("data/star.png"));
  m_textureGun = new QOpenGLTexture(QImage("data/gun.png"));

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

void GLWidget::Update(float elapsedSeconds)
{
  float const kSpeed = 20.0f; // pixels per second.

  auto position = Player.GetPosition();

  if (m_directions[kUpDirection])
    position.setY(position.y() + kSpeed * elapsedSeconds);
  if (m_directions[kDownDirection])
    position.setY(position.y() - kSpeed * elapsedSeconds);
  if (m_directions[kLeftDirection])
    position.setX(position.x() - kSpeed * elapsedSeconds);
  if (m_directions[kRightDirection])
    position.setX(position.x() + kSpeed * elapsedSeconds);

  for (auto const & i: GameManager.GetBulletList())
  {
    i->Move(elapsedSeconds);
  }

  Player.SetPosition(position);
}

void GLWidget::Render()
{
  m_texturedRect->Render(m_textureGun, Player.GetPosition(), QSize(64, 64), m_screenSize);

  for (auto const & i: GameManager.GetAlienList())
    m_texturedRect->Render(m_textureAlien, i->GetPosition(), QSize(64, 64), m_screenSize);
  for (auto const & i: GameManager.GetBulletList())
  {
    if (i->GetPosition().x() < 0 || i->GetPosition().x() > m_screenSize.width() ||
        i->GetPosition().y() < 0 || i->GetPosition().y() > m_screenSize.height())
      i->SetActive(false);
    m_texturedRect->Render(m_textureBullet, i->GetPosition(), QSize(16, 16), m_screenSize);
  }

  GameManager.Intersections();

}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QOpenGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
    auto bulletObj = GameFactory.Create(GameObjectsTypes::Bullet);
    bulletObj->SetPosition(Player.GetPosition());
    auto bullet = static_cast<Bullet*>(bulletObj);
    bullet->SetDirection(QVector2D(px, py) - Player.GetPosition());
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
}
