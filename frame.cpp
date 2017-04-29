#include "drawable.h"
#include "frame.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

Frame::Frame( SDL_Texture* tex ) :
  renderer(RenderContext::getInstance()->getRenderer()),
  texture( tex ),
  width(0),
  height(0),
  sourceX(0),
  sourceY(0)
{ SDL_QueryTexture(texture, NULL, NULL, &width, &height); }

Frame::Frame( const Frame& frame ) :
  renderer(frame.renderer),
  texture(frame.texture),
  width(frame.width),
  height(frame.height),
  sourceX(frame.sourceX),
  sourceY(frame.sourceY)
{ }


Frame& Frame::operator=(const Frame& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  width = rhs.width;
  height = rhs.height;
  sourceX = rhs.sourceX;
  sourceY = rhs.sourceY;
  return *this;
}

void Frame::draw(int x, int y) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  SDL_Rect dest = {x, y, width, height };
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Frame::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = height * scale;
  int tempWidth = width * scale;
  SDL_Rect dest = {x, y, tempWidth, tempHeight };
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}


void Frame::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, width, height };
  SDL_Rect dst = { dx, dy, width, height };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Frame* Frame::crop(SDL_Rect sub) const{
  if(sub.x+sub.w > width || sub.y+sub.h > height){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<width << "x"<<height<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Frame* cloned = new Frame(*this);
  cloned->width = sub.w;
  cloned->height = sub.h;

  return cloned;
}
