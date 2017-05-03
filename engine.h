#include <vector>
#include <SDL.h>
#include "ioMod.h"
//#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "twoWaySprite.h"
#include "SubjectSprite.h"
#include "player.h"
#include "hud.h"


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
  HUD *hud;
  bool hudOn;
  bool godMode;
  bool newGame;

  std::vector<SubjectSprite*> sprites;
  std::vector<Drawable*> extras;
  std::vector<Drawable*> nontracker_sprite;
  //Player player;

  int currentSprite;
  bool makeVideo;

  void makeExtras();
  void draw() const;
  void update(Uint32);
  void checkForCollisions();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};
