#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);


  virtual void update(Uint32 ticks);
  virtual void draw() const;
   virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }
  void stop();
  void right();
  void left();
  void up();
  void down();

protected:
  std::vector<Frame *> frames;
  const std::vector<Frame *> framesLeft;
  const std::vector<Frame *> framesRight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;

  Vector2f initialVelocity;
  float slowDown;
  Player& operator=(const Player&);

  void advanceFrame(Uint32 ticks);
};
#endif
