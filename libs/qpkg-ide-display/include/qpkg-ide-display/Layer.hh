#pragma once

#include <memory>
#include <qpkg-ide-utility/Layer.hh>

namespace qpkg::ide::display {
  class IDisplayLayer final {
protected:
    static std::atomic<bool> m_initialized;
    static IDisplayLayer *m_instance;

    enum class AppState {
      INIT_ANIMATION_START,
      INIT_ANIMATION_DONE,
      INIT_DONE,
    } m_app_state;

    struct Impl;

    std::unique_ptr<utility::IUtilityLayer> m_utility_api;
    std::unique_ptr<Impl> m_impl;

    virtual bool init_layer();
    virtual bool deinit_layer();

    virtual void spawn_background_tasks();
    virtual void stop_background_tasks();

    virtual bool do_animate_opening();
    virtual void application_update();
    virtual void update_state();
    virtual void render_state();

    uint32_t time_left();

public:
    IDisplayLayer(std::unique_ptr<utility::IUtilityLayer> utility);
    ~IDisplayLayer();

    virtual void start_application();
    virtual void stop_application();

    static IDisplayLayer &Get();
  };
} // namespace qpkg::ide::display
