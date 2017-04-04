#include "hud.h"


HUD::HUD() :
  rect( new SDL_Rect()),
  renderContext( RenderContext::getInstance()){
    rect->x = 12;
    rect->y = 16;
    rect->w = 245;
    rect->h = 155;
}


void HUD::drawHUD(){
  SDL_Renderer *renderer = renderContext->getRenderer();

  // Set render color to red ( background will be rendered in this color )
  SDL_SetRenderDrawColor( renderer, 208, 209, 210, 255 );

  // Clear winow
  //SDL_RenderClear( renderer );

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

  // Render rect
  SDL_RenderFillRect( renderer, rect );

  SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
  SDL_RenderDrawRect( renderer, rect );

  // Render the rect to the screen
  //SDL_RenderPresent(renderer);
}
