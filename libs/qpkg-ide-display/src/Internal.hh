#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <mutex>
#include <qpkg-ide-display/Layer.hh>
#include <set>
#include <unordered_set>

namespace qpkg::ide::display {
  struct LeftMenuState {
    int hover_select_index = -1;
    int open_select_index = -1;
  };

  enum class MouseButton {
    LEFT,
    MIDDLE,
    RIGHT,
  };

  enum class MouseState {
    UP,
    DOWN,
    HOVER,
  };

  typedef std::function<void(MouseButton, MouseState, int, int)> MouseCallback;
  class MouseObserver {
    struct Comp {
      using T = MouseCallback;
      bool operator()(const T &lhs, const T &rhs) const { return &lhs < &rhs; }
    };

    std::mutex m_mutex;
    std::set<MouseCallback, Comp> m_callbacks;

public:
    MouseObserver() = default;
    ~MouseObserver() = default;

    void add_callback(MouseCallback callback) {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_callbacks.insert(callback);
    }

    void notify(MouseButton button, MouseState state, int x, int y) {
      std::lock_guard<std::mutex> lock(m_mutex);

      for (auto &callback : m_callbacks) {
        callback(button, state, x, y);
      }
    }
  };

  struct qpkg::ide::display::IDisplayLayer::Impl {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    int conf_width = 0, conf_height = 0;
    int width = 0, height = 0;
    uint32_t next_time = 0;
    int fps = 0;
    float mspf = 0;
    bool running = false;
    float zoom = 1, last_zoom = 1;
    LeftMenuState left_menu_state;
    MouseObserver mouse_observer;
  };

} // namespace qpkg::ide::display