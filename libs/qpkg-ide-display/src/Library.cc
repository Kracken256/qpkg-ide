#include <Internal.hh>
#include <cassert>
#include <iostream>
#include <qpkg-ide-display/Layer.hh>
#include <thread>

#include <SDL2/SDL.h>

#define ZOOM_STEP 0.1

/* ==================== Static Members Initialization ==================== */
std::atomic<bool> qpkg::ide::display::IDisplayLayer::m_initialized;
qpkg::ide::display::IDisplayLayer *qpkg::ide::display::IDisplayLayer::m_instance;

qpkg::ide::display::IDisplayLayer::IDisplayLayer(
    std::unique_ptr<qpkg::ide::utility::IUtilityLayer> display) {
  assert(!m_initialized);

  m_utility_api = std::move(display);
  m_impl = std::make_unique<Impl>();
  m_instance = this;

  if (!init_layer()) {
    throw std::runtime_error("Failed to initialize display layer.");
  }
}

qpkg::ide::display::IDisplayLayer::~IDisplayLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize display layer." << std::endl;
  }

  m_instance = nullptr;
}

bool qpkg::ide::display::IDisplayLayer::init_layer() {
  /* ==================== State Initialization ==================== */
  m_app_state = AppState::INIT_ANIMATION_START;
  m_impl->fps = 120;
  m_impl->mspf = 1000.0 / m_impl->fps;
  m_impl->conf_width = 1024;
  m_impl->conf_height = 768;
  m_impl->zoom = 1.0;

  /* ==================== SDL2 Initialization ==================== */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  /* ==================== Window and Renderer Initialization ==================== */
  m_impl->window = SDL_CreateWindow(
      "QPKG Integrations", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_impl->conf_width,
      m_impl->conf_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

  if (m_impl->window == nullptr) {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    return false;
  }

  m_impl->renderer = SDL_CreateRenderer(m_impl->window, -1, SDL_RENDERER_ACCELERATED);
  if (m_impl->renderer == nullptr) {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_GetWindowSize(m_impl->window, &m_impl->conf_width, &m_impl->conf_height);
  std::cerr << "[+] Display " << m_impl->conf_width << "x" << m_impl->conf_height << " initialized."
            << std::endl;

  /* ==================== Window Configuration ==================== */
  SDL_SetWindowMinimumSize(m_impl->window, 720, 480);

  /* ==================== Render Init ==================== */
  SDL_SetRenderDrawColor(m_impl->renderer, 12, 13, 20, 255);
  SDL_RenderClear(m_impl->renderer);
  SDL_RenderPresent(m_impl->renderer);

  std::cerr << "[+] Display layer initialized." << std::endl;
  m_initialized = true;
  return true;
}

bool qpkg::ide::display::IDisplayLayer::deinit_layer() {
  SDL_DestroyRenderer(m_impl->renderer);
  SDL_DestroyWindow(m_impl->window);

  SDL_Quit();

  std::cerr << "[+] Display layer deinitialized." << std::endl;
  m_initialized = false;
  return true;
}

uint32_t qpkg::ide::display::IDisplayLayer::time_left() {
  uint32_t now;

  now = SDL_GetTicks();
  if (m_impl->next_time <= now)
    return 0;
  else
    return m_impl->next_time - now;
}

void qpkg::ide::display::IDisplayLayer::update_state() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      std::cerr << "Quitting application." << std::endl;
      m_impl->running = false;
      break;
    }
    case SDL_WINDOWEVENT: {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        m_impl->conf_width = event.window.data1;
        m_impl->conf_height = event.window.data2;
        m_impl->width = m_impl->conf_width / m_impl->zoom;
        m_impl->height = m_impl->conf_height / m_impl->zoom;
        break;
      case SDL_WINDOWEVENT_RESIZED:
        m_impl->conf_width = event.window.data1;
        m_impl->conf_height = event.window.data2;
        m_impl->width = m_impl->conf_width / m_impl->zoom;
        std::cerr << "Window resized to " << m_impl->width << "x" << m_impl->height << std::endl;
        break;
      default:
        break;
      }
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_EQUALS && (event.key.keysym.mod & KMOD_CTRL)) {
        std::cerr << "Ctrl + + pressed." << std::endl;
        if (m_impl->zoom < 2.6) {
          std::cerr << "Zooming @ " << (int)(m_impl->zoom * 100) << "%" << std::endl;
          m_impl->zoom += ZOOM_STEP;
        } else {
          std::cerr << "Cannot zoom in further." << std::endl;
        }
        break;
      }

      if (event.key.keysym.sym == SDLK_MINUS && (event.key.keysym.mod & KMOD_CTRL)) {
        if (m_impl->zoom > 0.5) {
          std::cerr << "Zooming @ " << (int)(m_impl->zoom * 100) << "%" << std::endl;
          m_impl->zoom -= ZOOM_STEP;
        } else {
          std::cerr << "Cannot zoom out further." << std::endl;
        }

        break;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      MouseState state = MouseState::DOWN;
      MouseButton button;

      if (event.button.button == SDL_BUTTON_LEFT) {
        button = MouseButton::LEFT;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        button = MouseButton::MIDDLE;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        button = MouseButton::RIGHT;
      } else {
        break;
      }
      m_impl->mouse_observer.notify(button, state, event.button.x, event.button.y);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      MouseState state = MouseState::UP;
      MouseButton button;

      if (event.button.button == SDL_BUTTON_LEFT) {
        button = MouseButton::LEFT;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        button = MouseButton::MIDDLE;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        button = MouseButton::RIGHT;
      } else {
        break;
      }
      m_impl->mouse_observer.notify(button, state, event.button.x, event.button.y);
      break;
    }
    case SDL_MOUSEMOTION: {
      MouseState state = MouseState::HOVER;
      MouseButton button;

      if (event.button.button == SDL_BUTTON_LEFT) {
        button = MouseButton::LEFT;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        button = MouseButton::MIDDLE;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        button = MouseButton::RIGHT;
      } else {
        break;
      }
      m_impl->mouse_observer.notify(button, state, event.button.x, event.button.y);
      break;
    }
    default:
      break;
    }
  }

  if (m_impl->last_zoom != m_impl->zoom) {
    m_impl->last_zoom = m_impl->zoom;
    SDL_RenderSetScale(m_impl->renderer, m_impl->zoom, m_impl->zoom);
    m_impl->width = m_impl->conf_width / m_impl->zoom;
    m_impl->height = m_impl->conf_height / m_impl->zoom;
  }
}

void qpkg::ide::display::IDisplayLayer::render_state() {
  SDL_SetRenderDrawColor(m_impl->renderer, 0, 0, 0, 255);

  switch (m_app_state) {
  case AppState::INIT_ANIMATION_START:
    if (!do_animate_opening()) { // false on done
      m_app_state = AppState::INIT_ANIMATION_DONE;
    }
    break;
  case AppState::INIT_ANIMATION_DONE:
    m_app_state = AppState::INIT_DONE;
    /* Init layers */
    init_zindex_layer0();
    break;
  case AppState::INIT_DONE:
    render_layers();
    break;
  default:
    assert(false);
    break;
  }
  SDL_RenderPresent(m_impl->renderer);
}

void qpkg::ide::display::IDisplayLayer::render_layers() {
  SDL_SetRenderDrawColor(m_impl->renderer, 9, 9, 11, 255);
  SDL_RenderClear(m_impl->renderer);

  render_zindex_layer0();
}

void qpkg::ide::display::IDisplayLayer::application_update() {
  /**
   * TODO: SUPPORT FOR EXTENSIONS??
   */

  update_state();
  render_state();
}

void qpkg::ide::display::IDisplayLayer::start_application() {
  spawn_background_tasks();
  m_impl->running = true;

  m_impl->next_time = SDL_GetTicks() + m_impl->mspf;
  while (m_impl->running) {
    application_update();
    SDL_Delay(time_left());
    m_impl->next_time += m_impl->mspf;
  }

  stop_application();
}

void qpkg::ide::display::IDisplayLayer::stop_application() {
  SDL_Event event{};
  event.type = SDL_QUIT;
  SDL_PushEvent(&event);

  stop_background_tasks();
}

void qpkg::ide::display::IDisplayLayer::spawn_background_tasks() {
  //
}

void qpkg::ide::display::IDisplayLayer::stop_background_tasks() {
  //
}

qpkg::ide::display::IDisplayLayer &qpkg::ide::display::IDisplayLayer::Get() {
  assert(m_initialized);
  assert(m_instance != nullptr);

  return *m_instance;
}
