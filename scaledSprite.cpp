#include <cmath>
#include "ScaledSprite.h"
#include "gamedata.h"
#include "renderContext.h"

ScaledSprite::ScaledSprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getRandInRange(
                        0, Gamedata::getInstance().getXmlInt("world/width")),
                    Gamedata::getInstance().getRandInRange(
                        0, Gamedata::getInstance().getXmlInt("world/height"))),
           Vector2f(Gamedata::getInstance().getRandInRange(
                        Gamedata::getInstance().getXmlInt(name + "/speed/X/min"),
                        Gamedata::getInstance().getXmlInt(name + "/speed/X/max")),
                    Gamedata::getInstance().getRandInRange(
                        Gamedata::getInstance().getXmlInt(name + "/speed/Y/min"),
                        Gamedata::getInstance().getXmlInt(name + "/speed/Y/max")))
           ),
  scale( Gamedata::getInstance().getRandInRange(
    Gamedata::getInstance().getXmlFloat(name + "/scale/min"),
    Gamedata::getInstance().getXmlFloat(name + "/scale/max") )
  ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight())
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s),
  scale(s.scale),
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight())
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  return *this;
}

void ScaledSprite::draw() const {
  frame->draw(getX(), getY(), scale);
}

void ScaledSprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }
}
