#pragma once

#include <memory>
#include <qpkg-ide-core/Layer.hh>

namespace qpkg::ide::business {
  class IBusinessLayer final {
protected:
    static std::atomic<bool> m_initialized;
    std::unique_ptr<core::ICoreLayer> m_core_api;

    bool init_layer();
    bool deinit_layer();

public:
    IBusinessLayer(std::unique_ptr<core::ICoreLayer> core);
    ~IBusinessLayer();

    virtual bool request_resource(std::string_view resource_name, std::string &out);
  };
} // namespace qpkg::ide::business
