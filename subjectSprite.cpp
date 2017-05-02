#include <iostream>
#include <cmath>
#include "subjectSprite.h"


CollisionStrategy* getStrat(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  //if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}

SubjectSprite::SubjectSprite( const std::string& name) :
  Player(name),
  observers(),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  strategy( getStrat(name) ),
  bullets( new BulletPool(bulletName) ),
  lives(5),
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
  	return strategy->execute(*this, *obj);
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

void SubjectSprite::hurt() {
	if(getX() > 100){
		setX(getX() - 50);
	}else{
		setX(10);
	}
	if (lives > 0){
		lives--;
	}
	if(lives == 0){
		//cue restart
	}
	std::cout << lives << '\n';
}
