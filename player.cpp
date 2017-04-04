#include "Player.h"
#include "gamedata.h"
#include "renderContext.h"
#include <cmath>

void Player::advanceFrame(Uint32 ticks) {
	if (frameDelay > frameInterval){
    incrFrame();
    frameDelay = ::fmod(frameDelay, frameInterval);
  }
  int magnitude = getVelocity().magnitude();
  if(magnitude < initialVelocity.magnitude()/2){
    frameDelay += minFrameDelay;
  }
  else frameDelay = frameDelay+ticks*magnitude/1000;
}

Player::Player( const std::string& name) :
  twoWaySprite(name),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  minFrameDelay(Gamedata::getInstance().getXmlFloat(name + "/minFrameDelay")),
  frameDelay(0),
  initialVelocity(getVelocity()),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown"))
{ }

Player::Player(const Player& s) :
  twoWaySprite(s),
  frameInterval( s.frameInterval ),
  minFrameDelay(s.minFrameDelay),
  frameDelay(s.frameDelay),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown)

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
