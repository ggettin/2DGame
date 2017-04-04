#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "twoWaySprite.h"

class Player : public twoWaySprite {
public:
  Player(const std::string&);
  Player(const Player&);


  virtual void update(Uint32 ticks);
  void stop();
  void right();
  void left();
  void up();
  void down();

protected:
  float frameInterval;
  float minFrameDelay;
  float frameDelay;

  Vector2f initialVelocity;
  const float slowDown;
  Player& operator=(const Player&);
  
  void advanceFrame(Uint32 ticks);
};
#endif
