#include "Player.h"
#include "gamedata.h"
#include "renderContext.h"
#include <cmath>

Player::Player( const std::string& name) :
  Drawable(name,
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
    // Vector2f(Gamedata::getInstance().getRandInRange(
    //              Gamedata::getInstance().getXmlInt(name + "/speed/X/min"),
    //              Gamedata::getInstance().getXmlInt(name + "/speed/X/max")),
    //          Gamedata::getInstance().getRandInRange(
    //              Gamedata::getInstance().getXmlInt(name + "/speed/Y/min"),
    //              Gamedata::getInstance().getXmlInt(name + "/speed/Y/max"))
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/speed/X/max"),
             Gamedata::getInstance().getXmlInt(name+"/speed/Y/max")),
    Gamedata::getInstance().getRandFloat(
               Gamedata::getInstance().getXmlFloat(name + "/scale/min"),
               Gamedata::getInstance().getXmlFloat(name + "/scale/max") )
    ),
  frames( RenderContext::getInstance()->getFrames("PepeRight") ),
  framesLeft( RenderContext::getInstance()->getFrames(name+"Left") ),
  framesRight( RenderContext::getInstance()->getFrames(name+"Right") ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  initialVelocity(
    Vector2f(Gamedata::getInstance().getXmlInt(name + "/speed/X/min"),
                 Gamedata::getInstance().getXmlInt(name + "/speed/Y/min")
             )),
  slowDown( Gamedata::getInstance().getXmlInt(name+"/slowDown") ) { }


Player::Player(const Player& s) :
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
  frameHeight( s.frameHeight ),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown)
  { }

// void Player::draw() const {
//   frames[currentFrame]->draw(getX(), getY());
// }
void Player::draw() const {
  frames[currentFrame]->draw(getX(), getY(), getScale());
}

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

void Player::stop(){
  setVelocityX(slowDown*getVelocityX());
  setVelocityY(0);
}

void Player::right(){
  if( getX() > 0){
    setVelocityX(-initialVelocity[0]);
  }
  frames = framesRight;
}
void Player::left(){
  if( getX() < worldWidth-frameWidth){
    setVelocityX(initialVelocity[0]);
  }
  frames = framesLeft;
}
void Player::up(){
   if( getY() < worldHeight-frameHeight){
    setVelocityY(initialVelocity[1]);
  }
}
void Player::down(){
  if( getY() > 0){
    setVelocityY(-initialVelocity[1]);
  }

}


void Player::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() <= 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() >= worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

	stop();

}
