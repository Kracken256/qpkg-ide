#include <cassert>
#include <iostream>
#include <qpkg-ide-business/Layer.hh>

std::atomic<bool> qpkg::ide::business::IBusinessLayer::m_initialized;

qpkg::ide::business::IBusinessLayer::IBusinessLayer(
    std::unique_ptr<qpkg::ide::core::ICoreLayer> core) {
  assert(!m_initialized);

  m_core_api = std::move(core);

  if (!init_layer()) {
    throw std::runtime_error("Failed to initialize business layer.");
  }
}

qpkg::ide::business::IBusinessLayer::~IBusinessLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize business layer." << std::endl;
  }
}

bool qpkg::ide::business::IBusinessLayer::init_layer() {
  /// TODO: Implement business layer initialization.

  std::cerr << "[+] Business layer initialized." << std::endl;
  m_initialized = true;
  return true;
}

bool qpkg::ide::business::IBusinessLayer::deinit_layer() {
  /// TODO: Implement business library deinitialization.

  std::cerr << "[+] Business layer deinitialized." << std::endl;
  m_initialized = false;
  return true;
}
