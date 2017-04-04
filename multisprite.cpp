#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
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
								Gamedata::getInstance().getXmlInt(name + "/speed/Y/max"))),
	Gamedata::getInstance().getRandFloat(
							    Gamedata::getInstance().getXmlFloat(name + "/scale/min"),
							    Gamedata::getInstance().getXmlFloat(name + "/scale/max") )
	 ),
  frames( RenderContext::getInstance()->getFrames(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

// void MultiSprite::draw() const {
//   frames[currentFrame]->draw(getX(), getY());
// }

void MultiSprite::draw() const {
  frames[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

}
