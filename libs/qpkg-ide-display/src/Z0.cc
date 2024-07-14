#include <Internal.hh>
#include <SDL2/SDL.h>
#include <cassert>
#include <iostream>
#include <qpkg-ide-display/Layer.hh>

void qpkg::ide::display::IDisplayLayer::render_left_sidebar() {

  /* Left sidebar outline */
  SDL_Rect rect = {0, 24, 40, m_impl->height - 24 - 18};
  utility::RGBA color = RGBA_FETCH("Z0_LSIDEBAR_FILL_COLOR");
  SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
  SDL_RenderFillRect(m_impl->renderer, &rect);

  /* Draw icons */

  const char *srcfile = GETPATH("Z0_LSIDEBAR_SOURCEFILE_ICON_BMP");
  const char *search = GETPATH("Z0_LSIDEBAR_SEARCH_ICON_BMP");
  const char *git = GETPATH("Z0_LSIDEBAR_GIT_ICON_BMP");
  const char *runanddebug = GETPATH("Z0_LSIDEBAR_RUN_AND_DEBUG_ICON_BMP");
  const char *extensions = GETPATH("Z0_LSIDEBAR_EXTENSIONS_ICON_BMP");
  const char *settings = GETPATH("Z0_LSIDEBAR_SETTINGS_ICON_BMP");

  SDL_Surface *bmp = nullptr;
  SDL_Texture *tex = nullptr;
  SDL_Rect src, dst;

#define PUT_ICON(__name, __x, __y, __idx)                                                          \
  if (m_impl->left_menu_state.hover_select_index == __idx) {                                       \
    rect = {0, __y - 8, 40, 40};                                                                   \
    color = RGBA_FETCH("Z0_LSIDEBAR_HOVER_FILL_COLOR");                                            \
    SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));                              \
    SDL_RenderFillRect(m_impl->renderer, &rect);                                                   \
  }                                                                                                \
  bmp = SDL_LoadBMP(__name);                                                                       \
  assert(bmp != nullptr);                                                                          \
  tex = SDL_CreateTextureFromSurface(m_impl->renderer, bmp);                                       \
  assert(tex != nullptr);                                                                          \
  src = {0, 0, bmp->w, bmp->h};                                                                    \
  dst = {__x, __y, 24, 24};                                                                        \
  SDL_RenderCopy(m_impl->renderer, tex, &src, &dst);                                               \
  SDL_FreeSurface(bmp);                                                                            \
  SDL_DestroyTexture(tex);

  PUT_ICON(srcfile, 8, 32, 0)
  PUT_ICON(search, 8, 72, 1)
  PUT_ICON(git, 8, 112, 2)
  PUT_ICON(runanddebug, 8, 152, 3)
  PUT_ICON(extensions, 8, 192, 4)
  PUT_ICON(settings, 8, m_impl->height - 50, 5)

  rect = {40, 24, 1, m_impl->height - 24 - 18};
  color = RGBA_FETCH("Z0_LSIDEBAR_SEP_COLOR");
  SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
  SDL_RenderFillRect(m_impl->renderer, &rect);

  if (m_impl->left_menu_state.open_select_index != -1) {
    switch (m_impl->left_menu_state.open_select_index) {
    case 0:
      rect = {0, 24, 2, 40};
      break;
    case 1:
      rect = {0, 64, 2, 40};
      break;
    case 2:
      rect = {0, 104, 2, 40};
      break;
    case 3:
      rect = {0, 144, 2, 40};
      break;
    case 4:
      rect = {0, 184, 2, 40};
      break;
    case 5:
      rect = {0, m_impl->height - 58, 2, 40};
      break;
    default:
      break;
    }

    color = RGBA_FETCH("Z0_LSIDEBAR_TAB_HIGH_COLOR");
    SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
    SDL_RenderFillRect(m_impl->renderer, &rect);

    rect = {41, 24, 250, m_impl->height - 18 - 24};
    color = RGBA_FETCH("Z0_LSIDEBAR_FILL_COLOR");
    SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
    SDL_RenderFillRect(m_impl->renderer, &rect);

    rect = {290, 24, 1, m_impl->height - 24 - 18};
    color = RGBA_FETCH("Z0_LSIDEBAR_SEP_COLOR");
    SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
    SDL_RenderFillRect(m_impl->renderer, &rect);
  }
}

void qpkg::ide::display::IDisplayLayer::render_top_bar() {
  /* Top bar */
  SDL_Rect rect = {0, 0, m_impl->width, 24};
  utility::RGBA color = RGBA_FETCH("Z0_TOP_BAR_FILL_COLOR");
  SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
  SDL_RenderFillRect(m_impl->renderer, &rect);
}

void qpkg::ide::display::IDisplayLayer::render_bottom_bar() {
  /* Bottom status bar */
  SDL_Rect rect = {0, m_impl->height - 18, m_impl->width, m_impl->height};
  utility::RGBA color = RGBA_FETCH("Z0_BOTTOM_BAR_FILL_COLOR");
  SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
  SDL_RenderFillRect(m_impl->renderer, &rect);

  rect = {0, m_impl->height - 18, m_impl->width, 1};
  color = RGBA_FETCH("Z0_LSIDEBAR_SEP_COLOR");
  SDL_SetRenderDrawColor(m_impl->renderer, SDL_RGBA_UNPACK(color));
  SDL_RenderFillRect(m_impl->renderer, &rect);
}

void qpkg::ide::display::IDisplayLayer::render_zindex_layer0() {
  /* ======================= DRAW Z0 ======================= */
  render_left_sidebar();
  render_top_bar();
  render_bottom_bar();
}

void qpkg::ide::display::IDisplayLayer::init_zindex_layer0() {
  m_impl->mouse_observer.add_callback([this](MouseButton button, MouseState state, int x, int y) {
    (void)button;
    
    y /= m_impl->zoom;
    x /= m_impl->zoom;

    if (state == MouseState::HOVER) {
      if (x > 40) {
        m_impl->left_menu_state.hover_select_index = -1;
        return;
      }

      if ((y - 25) / 40 == 0) {
        m_impl->left_menu_state.hover_select_index = 0;
      } else if ((y - 25) / 40 == 1) {
        m_impl->left_menu_state.hover_select_index = 1;
      } else if ((y - 25) / 40 == 2) {
        m_impl->left_menu_state.hover_select_index = 2;
      } else if ((y - 25) / 40 == 3) {
        m_impl->left_menu_state.hover_select_index = 3;
      } else if ((y - 25) / 40 == 4) {
        m_impl->left_menu_state.hover_select_index = 4;
      } else if (y > m_impl->height - 50) {
        m_impl->left_menu_state.hover_select_index = 5;
      }

    } else if (state == MouseState::DOWN) {
      if (x > 40) {
        return;
      }

      int last = m_impl->left_menu_state.open_select_index;

      if ((y - 25) / 40 == 0) {
        m_impl->left_menu_state.open_select_index = 0;
      } else if ((y - 25) / 40 == 1) {
        m_impl->left_menu_state.open_select_index = 1;
      } else if ((y - 25) / 40 == 2) {
        m_impl->left_menu_state.open_select_index = 2;
      } else if ((y - 25) / 40 == 3) {
        m_impl->left_menu_state.open_select_index = 3;
      } else if ((y - 25) / 40 == 4) {
        m_impl->left_menu_state.open_select_index = 4;
      } else if (y > m_impl->height - 50) {
        m_impl->left_menu_state.open_select_index = 5;
      }

      if (last == m_impl->left_menu_state.open_select_index) {
        m_impl->left_menu_state.open_select_index = -1;
      }
    }
  });
}