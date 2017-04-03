#ifndef ScaledSprite__H
#define ScaledSprite__H
#include <string>
#include "drawable.h"

class ScaledSprite : public Drawable {
public:
  ScaledSprite(const std::string&);
  ScaledSprite(const ScaledSprite&);
  virtual ~ScaledSprite() { }
  ScaledSprite& operator=(const ScaledSprite&);

  virtual const Frame* getFrame() const { return frame; }
  float getScale() const { return scale; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  Vector2f makeLocation();
private:
  double scale;
  SDL_Surface *scaledSurface;
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const ScaledSprite*) const;
};
#endif
