#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "scaledSprite.h"
#include "twoWaySprite.h"
#include "Player.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World water;
  World coral;
  Viewport& viewport;

  std::vector<Player*> sprites;
  std::vector<ScaledSprite*> extras;
  std::vector<Drawable*> nontracker_sprite;

  int currentSprite;
  bool makeVideo;

  void makeExtras();
  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};
