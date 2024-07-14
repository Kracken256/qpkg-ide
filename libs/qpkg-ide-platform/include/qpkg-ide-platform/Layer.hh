#pragma once

#include <atomic>
#include <memory>
#include <string_view>
#include <vector>

namespace qpkg::ide::platform {
  class IPlatformLayer final {
protected:
    static std::atomic<bool> m_initialized;

    bool init_layer();
    bool deinit_layer();

public:
    IPlatformLayer(std::vector<std::string_view> args);
    ~IPlatformLayer();

    std::string_view app_install_dir() const;
  };
} // namespace qpkg::ide::platform
