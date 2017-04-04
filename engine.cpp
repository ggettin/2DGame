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

class ScaleComp {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};



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
  for(auto it: nontracker_sprite){
    delete it;
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
  nontracker_sprite(),
  currentSprite(-1),

  makeVideo( false ){

  makeExtras();

  for( int i = 0; i < Gamedata::getInstance().getXmlInt("jellyFish/count"); i++){
    nontracker_sprite.push_back( new MultiSprite("jellyFish") );
  }
  for( int i = 0; i < Gamedata::getInstance().getXmlInt("shark/count"); i++){
    nontracker_sprite.push_back( new twoWaySprite("shark") );
  }
  sprites.push_back( new twoWaySprite("scuba") );

  switchSprite();
  std::cout << "Loading complete" << std::endl;
}


void Engine::makeExtras(){
  int count = Gamedata::getInstance().getXmlInt("bubble/count");
  extras.reserve(count);
  for( int i = 0; i < count; i++){
    extras.push_back( new ScaledSprite("bubble") );
  }
  sort(extras.begin(), extras.end(), ScaleComp());
}

void Engine::draw() const {
  water.draw();
  size_t i = 0;
  while (i < extras.size()){
    if(extras[i]->getScale() >= 0.8){
      break;
    }
    extras[i]->draw();
    i++;
  }
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
  while(i < extras.size()){
    extras[i]->draw();
    i++;
  }
  for(auto* f : nontracker_sprite) f->draw();


  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* e : extras) e->update(ticks);
  for(auto* f : nontracker_sprite) f->update(ticks);

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
        // if ( keystate[SDL_SCANCODE_S] ) {
        //   clock.toggleSloMo();
        // }
        if ( keystate[SDL_SCANCODE_T] ) {
          //switchSprite();
        }
        if ( keystate[SDL_SCANCODE_B] ) {
          extras.push_back( new ScaledSprite("bubble") );
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

      if (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D]){
        std::cout << "a & d" << std::endl;

      } else if (keystate[SDL_SCANCODE_A]){
        std::cout << "a " << std::endl;

      } else if (keystate[SDL_SCANCODE_D]){
        std::cout << "d" << std::endl;

      }
      if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S]){
        std::cout << "w & s" << std::endl;

      }else if (keystate[SDL_SCANCODE_W]){
        std::cout << "w" << std::endl;

      }else if (keystate[SDL_SCANCODE_S]){
        std::cout << "s" << std::endl;

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
