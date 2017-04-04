#include <iostream>
#include "gamedata.h"
#include "renderContext.h"


class HUD{

public:
  HUD();
  ~HUD();

private:

  SDL_Rect *rect;
  RenderContext *renderContext;


};
