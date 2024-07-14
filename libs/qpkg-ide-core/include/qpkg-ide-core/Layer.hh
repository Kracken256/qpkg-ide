#pragma once

#include <memory>
#include <qpkg-ide-platform/Layer.hh>

namespace qpkg::ide::core {
  enum class ResourceType {
    JsonData,
    BinaryData,
    MediaData,
  };

  class ICoreLayer final {
protected:
    static std::atomic<bool> m_initialized;
    std::unique_ptr<platform::IPlatformLayer> m_platform_api;

    bool init_layer();
    bool deinit_layer();

public:
    ICoreLayer(std::unique_ptr<platform::IPlatformLayer> platform);
    ~ICoreLayer();

    virtual bool request_resource(std::string_view resource_id, ResourceType type,
                                  std::string &out);
  };

} // namespace qpkg::ide::core
