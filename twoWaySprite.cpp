#include "twoWaySprite.h"
#include "gamedata.h"
#include "renderContext.h"

void twoWaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

twoWaySprite::twoWaySprite( const std::string& name) :
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
  frames( RenderContext::getInstance()->getFrames("sharkLeft") ),
	framesLeft( RenderContext::getInstance()->getFrames(name+"Left") ),
	framesRight( RenderContext::getInstance()->getFrames(name+"Right") ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

twoWaySprite::twoWaySprite(const twoWaySprite& s) :
  Drawable(s),
  frames(s.frames),
	framesLeft(s.framesLeft), framesRight(s.framesRight),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void twoWaySprite::draw() const {
  frames[currentFrame]->draw(getX(), getY());
}

void twoWaySprite::update(Uint32 ticks) {
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

	if ( getVelocityX( ) >  0 ){
		frames = framesRight;
	}else{
		frames = framesLeft;
	}

}
