#include <iostream>
#include "multiSprite.h"
#include "bulletPool.h"
// #include "ioMod.h"


// SmartSprite is an observer of subject SubjectSprite, which
// in this example is a spinning star.

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string&);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  static void incrSafeDistance() {++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

  bool collidedWith(const Drawable* d) const;

private:
  enum MODE {NORMAL, ATTACK};
  // IOmod& io;
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  CollisionStrategy* strategy;
  static float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);

};
