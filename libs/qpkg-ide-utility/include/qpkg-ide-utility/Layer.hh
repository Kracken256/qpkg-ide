#pragma once

#include <memory>
#include <qpkg-ide-business/Layer.hh>

namespace qpkg::ide::utility {
  class IUtilityLayer final {
protected:
    static std::atomic<bool> m_initialized;
    std::unique_ptr<business::IBusinessLayer> m_business_api;

    bool init_layer();
    bool deinit_layer();

public:
    IUtilityLayer(std::unique_ptr<business::IBusinessLayer> business);
    ~IUtilityLayer();
  };
} // namespace qpkg::ide::utility
