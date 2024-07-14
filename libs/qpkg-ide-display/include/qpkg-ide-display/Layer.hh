#pragma once

#include <memory>
#include <qpkg-ide-utility/Layer.hh>

namespace qpkg::ide::display {
  class IDisplayLayer final {
    static std::atomic<bool> m_initialized;
    std::unique_ptr<utility::IUtilityLayer> m_utility_api;

    bool init_layer();
    bool deinit_layer();

public:
    IDisplayLayer(std::unique_ptr<utility::IUtilityLayer> utility);
    ~IDisplayLayer();

    void start_application();
  };
} // namespace qpkg::ide::display
