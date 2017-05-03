#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "smartSprite.h"
#include "player.h"
// #include "bulletPool.h"

class SubjectSprite : public Player {
public:
  SubjectSprite(const std::string&);
  // SubjectSprite(const std::string&, const Vector2f& pos,
  //               const Vector2f& vel, const std::vector<Frame*>& fms);
  SubjectSprite(const SubjectSprite&);

  virtual void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );
  void shoot();
  void hurt();
  virtual void draw() const;
  virtual bool collideWith(const Drawable*) const;
  BulletPool *getBullets() const {return bullets;}
  int getLives() const {return lives;}
  void reset();

private:
  std::list<SmartSprite*> observers;
  std::string bulletName;
  CollisionStrategy* strategy;
  BulletPool *bullets;
  int lives;
  float minSpeed;
  SubjectSprite& operator=(const SubjectSprite&);

};
