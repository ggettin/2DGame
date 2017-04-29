#include <iostream>
#include <cmath>
#include "subjectSprite.h"

SubjectSprite::SubjectSprite( const std::string& name) :
  Player(name),
  observers(),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets( new BulletPool(bulletName) ),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") )
{ }

// SubjectSprite::SubjectSprite(const std::string& name,
//                          const Vector2f& pos, const Vector2f& vel,
//                          const std::vector<Frame*>& fms) :
//   MultiSprite(name, pos, vel, fms),
//   observers()
// { }

SubjectSprite::SubjectSprite(const SubjectSprite& s) :
  Player(s),
  observers(s.observers),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed)
  { }

void SubjectSprite::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void SubjectSprite::draw() const {
  Player::draw();
  bullets->draw();
}

void SubjectSprite::shoot() {
  float x = getX()+getFrame()->getWidth();
  float y = getY()+getFrame()->getHeight()/2;
  // I'm not adding minSpeed to y velocity:
  bullets->shoot( Vector2f(x, y),
    Vector2f(minSpeed+getVelocityX(), 0)
  );
}

bool SubjectSprite::collideWith(const Drawable* obj) const {
  return bullets->collidedWith( obj );
}

void SubjectSprite::update(Uint32 ticks) {
  Player::update(ticks);
  bullets->update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
}
