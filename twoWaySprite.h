#ifndef twoWaySprite__H
#define twoWaySprite__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class twoWaySprite : public Drawable {
public:
  twoWaySprite(const std::string&);
  twoWaySprite(const twoWaySprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }

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

  void advanceFrame(Uint32 ticks);
};
#endif
