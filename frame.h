#ifndef FRAME__H
#define FRAME__H

#include <iostream>
#include <string>
#include <SDL.h>

class Frame {
public:
  Frame( SDL_Texture*);
  Frame(const Frame&);
  Frame& operator=(const Frame&);

  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  void draw(int sx, int sy, int dx, int dy) const;

  SDL_Texture* getTexture() const { return texture; }
  int getWidth()  const { return width; }
  int getHeight() const { return height; }
  Uint16 getSourceX() const { return sourceX; }
  Uint16 getSourceY() const { return sourceY; }

  Frame* crop(SDL_Rect)const;

private:
  SDL_Renderer * renderer;
  SDL_Texture * texture;
  int width;
  int height;
  Frame();
  Sint16 sourceX;
  Sint16 sourceY;

};

#endif
