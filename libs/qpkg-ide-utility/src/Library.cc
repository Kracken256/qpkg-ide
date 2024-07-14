#include <cassert>
#include <iostream>
#include <qpkg-ide-utility/Layer.hh>

std::atomic<bool> qpkg::ide::utility::IUtilityLayer::m_initialized;

qpkg::ide::utility::IUtilityLayer::IUtilityLayer(
    std::unique_ptr<qpkg::ide::business::IBusinessLayer> business) {
  assert(!m_initialized);

  m_business_api = std::move(business);

  if (!init_layer()) {
    throw std::runtime_error("Failed to initialize utility layer.");
  }
}

qpkg::ide::utility::IUtilityLayer::~IUtilityLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize utility layer." << std::endl;
  }
}

bool qpkg::ide::utility::IUtilityLayer::init_layer() {
  /// TODO: Implement utility layer initialization.

  std::cerr << "[+] Utility layer initialized." << std::endl;
  m_initialized = true;
  return true;
}

bool qpkg::ide::utility::IUtilityLayer::deinit_layer() {
  /// TODO: Implement utility library deinitialization.

  std::cerr << "[+] Utility layer deinitialized." << std::endl;
  m_initialized = false;
  return true;
}
