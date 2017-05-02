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
#include "audio.h"
#include "explodingSprite.h"


class ScaleComp {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};



Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  // for(auto sprite : sprites){
  //   delete sprite;
  // }
  std::vector<SubjectSprite*>::iterator it = sprites.begin();

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
	hud( new HUD()),
	hudOn(true),
	godMode(false),
	sprites(),
	nontracker_sprite(),
	//player("scuba"),
	currentSprite(-1),



	makeVideo( false )
{



	// for( int i = 0; i < Gamedata::getInstance().getXmlInt("jellyFish/count"); i++){
	//   nontracker_sprite.push_back( new MultiSprite("jellyFish") );
	// }
	// for( int i = 0; i < Gamedata::getInstance().getXmlInt("shark/count"); i++){
	//   //sprites.push_back( new twoWaySprite("shark") );
	// }
	sprites.push_back( new SubjectSprite("Pepe") );
	makeExtras();
	switchSprite();
	std::cout << "Loading complete" << std::endl;
}


void Engine::makeExtras(){
  int bubbleCount = Gamedata::getInstance().getXmlInt("bubble/count");
  int jellyFishCount = Gamedata::getInstance().getXmlInt("jellyFish/count");

  extras.reserve(bubbleCount + jellyFishCount);

  for( int i = 0; i < bubbleCount; i++){
    extras.push_back( new Sprite("bubble") );
  }

  for( int i = 0; i < jellyFishCount; i++){
    Drawable *tempSprite = new SmartSprite("jellyFish");
    if (tempSprite->getScale() >= 0.7) {
      nontracker_sprite.push_back(tempSprite);
		static_cast<SubjectSprite*>(sprites[0])->attach( (SmartSprite*)tempSprite );
    }else{
      extras.push_back( tempSprite );
    }
  }
  sort(extras.begin(), extras.end(), ScaleComp());
}

void Engine::draw() const {
  water.draw();
  size_t i = 0;
  while (i < extras.size()){
    if(extras[i]->getScale() >= 0.7){
      break;
    }
    extras[i]->draw();
    i++;
  }
  coral.draw();

  //int fps = clock.getFps();
  if (hudOn) {

    hud->drawHUD();
    //viewport.draw();
    std::stringstream strm;
    strm << "Pepe the Scuba Diver";
    io.writeText(strm.str(), 20, 20);
    // strm << "Seconds: " << clock.getSeconds();
    // io.writeText(strm.str(), 20, 50);
    // strm.str(std::string());
    // strm << "Avg. FPS: " << clock.calcAvgFPS(fps);
    // io.writeText(strm.str(), 20, 80);
    //strm.str(std::string());
	 strm.str(std::string());
	 strm << "Lives: " << sprites[0]->getLives();
    io.writeText(strm.str(), 20, 110);
	 strm.str(std::string());
	 strm << "[G]Mode: ";
	 if(godMode) strm << "on"; else strm << "on";
    io.writeText(strm.str(), 110, 110);
	 strm.str(std::string());
	 strm << "Up: W" ;
    io.writeText(strm.str(), 20, 140);
    strm.str(std::string());
    strm << "Left: A" ;
    io.writeText(strm.str(), 20, 170);
    strm.str(std::string());
    strm << "Down: S" ;
    io.writeText(strm.str(), 150, 140);
    strm.str(std::string());
    strm << "Right: D" ;
    io.writeText(strm.str(), 150, 170);

  }



  for(auto* s : sprites) s->draw();
  while(i < extras.size()){
    extras[i]->draw();
    i++;
  }
  for(auto* f : nontracker_sprite) f->draw();

  //player.draw();


  SDL_Color color = {0xff, 0xff, 0xff, 0};
  io.writeText("Greg Gettings & Sydney Paul", color, 20, 440);
  //io.writeText("Pepe the Scuba Diver", color, 50, 425);


  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* e : extras) e->update(ticks);
  for(auto* f : nontracker_sprite) f->update(ticks);
 // player.update(ticks);

  checkForCollisions();
  water.update();
  coral.update();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions(){
	// sprites[0] is the spinning star:
	SubjectSprite* player = static_cast<SubjectSprite*>( sprites[0] );
	BulletPool *bullets = player->getBullets();

	for (unsigned i = 0; i < nontracker_sprite.size(); ++i) {
		ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(nontracker_sprite[i]);
		SmartSprite* sprite = static_cast<SmartSprite*>( nontracker_sprite[i] );
		if ( e && e->chunkCount() == 0 ) {
			// Make a smart sprite
			nontracker_sprite[i] = new SmartSprite(sprite->getName());
			// delete sprite;
		}
		if ( !e && bullets->collidedWith(sprite) ) {
			nontracker_sprite[i] = new ExplodingSprite(sprite);
			// delete sprite;
		}
		if( !e && player->collideWith(sprite)){
			std::cout << "Pepe got hurt" << '\n';
			player->hurt();
		}
	}

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

  if(SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    return;
  }
  audio *aud = new audio();

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
        if ( keystate[SDL_SCANCODE_F1] ) {
            hudOn = !hudOn;
        }
		  if ( keystate[SDL_SCANCODE_G] ) {
            godMode = !godMode;
        }

        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }

        if ( keystate[SDL_SCANCODE_B] ) {

          aud->playSound("sounds/fishbubbles.wav", SDL_MIX_MAXVOLUME / 2);
          SDL_Delay(100);
          /* End Simple-SDL2-Audio */


          int additions = Gamedata::getInstance().getXmlInt("bubble/additions");
          for (int i = 0; i < additions; i++){
            extras.push_back( new Sprite("bubble") );
          }
          sort(extras.begin(), extras.end(), ScaleComp());

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
        sprites[0]->stop();


      } else if (keystate[SDL_SCANCODE_A]){
        sprites[0]->left();

      } else if (keystate[SDL_SCANCODE_D]){
        sprites[0]->right();

      }
      if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S]){
        sprites[0]->stop();

      }else if (keystate[SDL_SCANCODE_W]){
        sprites[0]->up();

      }else if (keystate[SDL_SCANCODE_S]){
        sprites[0]->down();

      }
		if ( keystate[SDL_SCANCODE_SPACE] ) {
			static_cast<SubjectSprite*>(sprites[0])->shoot();
			 std::cout << "shooting" << std::endl;
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
