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
  };
} // namespace qpkg::ide::platform
