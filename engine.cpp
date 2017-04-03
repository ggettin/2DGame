#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWaySprite.h"
#include "scaledSprite.h"

Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  // for(auto sprite : sprites){
  //   delete sprite;
  // }
  std::vector<Drawable*>::iterator it = sprites.begin();

  while ( it != sprites.end() ) {
    delete *it;
    ++it;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  water("water", Gamedata::getInstance().getXmlInt("water/factor") ),
  coral("coral", Gamedata::getInstance().getXmlInt("coral/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(-1),

  makeVideo( false ){

  for( int i = 0; i < Gamedata::getInstance().getXmlInt("bubble/count"); i++){
    sprites.push_back( new ScaledSprite("bubble") );
  }

  for( int i = 0; i < Gamedata::getInstance().getXmlInt("jellyFish/count"); i++){
    sprites.push_back( new MultiSprite("jellyFish") );
  }
  for( int i = 0; i < Gamedata::getInstance().getXmlInt("shark/count"); i++){
    sprites.push_back( new twoWaySprite("shark") );
  }
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  water.draw();
  coral.draw();
  std::stringstream strm;
  int fps = clock.getFps();
  strm << "fps: " << fps;
  io.writeText(strm.str(), 30, 60);
  SDL_Color color = {0xff, 0, 0, 0};
  io.writeText("Greg Gettings", color, 410, 30);
  io.writeText("Livin' Underwater", color, 395, 60);
  strm.str(std::string());
  strm << "avg. fps: " << clock.calcAvgFPS(fps);
  io.writeText(strm.str(), 30, 90);

  for(auto* s : sprites) s->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  water.update();
  coral.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  while(sprites[currentSprite]->getName() == "bubble"){
    ++currentSprite;
    currentSprite = currentSprite % sprites.size();
  }
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_S] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_B] ) {
          sprites.push_back( new Sprite("bubble") );
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
