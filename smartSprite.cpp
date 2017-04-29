#include <cmath>
#include <sstream>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name) :
 MultiSprite(name),
 // io(IOmod::getInstance()),
 // playerPos(),
 // playerWidth(w),
 // playerHeight(h),
 currentMode(NORMAL)
{ }

void SmartSprite::goLeft()  {
  if (getX() > 0) setVelocityX( -fabs(getVelocityX()) );
}
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) ); }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) ); }

bool SmartSprite::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
}

void SmartSprite::draw() const {
  MultiSprite::draw();
  // int x = 10+ getX() - Viewport::getInstance().getX();
  // int y = 10+ getY() - Viewport::getInstance().getY();
  // std::stringstream strm;
  // strm << currentMode;
  // io.printMessageBlackAt( strm.str(), x, y);
}

void SmartSprite::update(Uint32 ticks) {
  MultiSprite::update(ticks);
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = ATTACK;
  }
  else if  ( currentMode == ATTACK ) {
    if(distanceToEnemy > safeDistance) currentMode = NORMAL;
    else {
      if ( x < ex ) goRight();
      if ( x > ex ) goLeft();
      if ( y < ey ) goDown();
      if ( y > ey ) goUp();
    }
  }
}
