#pragma once

#include <memory>
#include <qpkg-ide-platform/Layer.hh>

namespace qpkg::ide::core {
  class ICoreLayer final {
    static std::atomic<bool> m_initialized;
    std::unique_ptr<platform::IPlatformLayer> m_platform_api;

    bool init_layer();
    bool deinit_layer();

public:
    ICoreLayer(std::unique_ptr<platform::IPlatformLayer> platform);
    ~ICoreLayer();
  };

} // namespace qpkg::ide::core