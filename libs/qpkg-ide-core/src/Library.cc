#include <cassert>
#include <iostream>
#include <qpkg-ide-core/Layer.hh>

std::atomic<bool> qpkg::ide::core::ICoreLayer::m_initialized;

qpkg::ide::core::ICoreLayer::ICoreLayer(
    std::unique_ptr<qpkg::ide::platform::IPlatformLayer> platform) {
  assert(!m_initialized);

  m_platform_api = std::move(platform);

  if (!init_layer()) {
    throw std::runtime_error("Failed to initialize core layer.");
  }
}

qpkg::ide::core::ICoreLayer::~ICoreLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize core layer." << std::endl;
  }
}

bool qpkg::ide::core::ICoreLayer::init_layer() {
  /// TODO: Implement core layer initialization.

  std::cerr << "[+] Core layer initialized." << std::endl;
  m_initialized = true;
  return true;
}

bool qpkg::ide::core::ICoreLayer::deinit_layer() {
  /// TODO: Implement core library deinitialization.

  std::cerr << "[+] Core layer deinitialized." << std::endl;
  m_initialized = false;
  return true;
}
