#pragma once

#include <SDL2/SDL.h>
#include <qpkg-ide-display/Layer.hh>

struct qpkg::ide::display::IDisplayLayer::Impl {
  SDL_Window *window;
  SDL_Renderer *renderer;

  int width, height;
  uint32_t next_time;
  int fps;
  float mspf;
  bool running;
};
