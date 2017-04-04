#include <iostream>
#include "renderContext.h"




class HUD{

public:
  HUD();
  ~HUD();

  void drawHUD();

private:

  SDL_Rect *rect;
  const RenderContext *renderContext;


};
