#include <cassert>
#include <iostream>
#include <qpkg-ide-display/Layer.hh>
#include <thread>

std::atomic<bool> qpkg::ide::display::IDisplayLayer::m_initialized;

qpkg::ide::display::IDisplayLayer::IDisplayLayer(
    std::unique_ptr<qpkg::ide::utility::IUtilityLayer> display) {
  assert(!m_initialized);

  m_utility_api = std::move(display);

  if (!init_layer()) {
    throw std::runtime_error("Failed to initialize display layer.");
  }
}

qpkg::ide::display::IDisplayLayer::~IDisplayLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize display layer." << std::endl;
  }
}

bool qpkg::ide::display::IDisplayLayer::init_layer() {
  /// TODO: Implement display layer initialization.

  std::cerr << "[+] Display layer initialized." << std::endl;
  m_initialized = true;
  return true;
}

bool qpkg::ide::display::IDisplayLayer::deinit_layer() {
  /// TODO: Implement display library deinitialization.

  std::cerr << "[+] Display layer deinitialized." << std::endl;
  m_initialized = false;
  return true;
}

void qpkg::ide::display::IDisplayLayer::start_application() {
  /// TODO: Implement display application start.

  for (int i = 0; i < 5; i++) {
    std::cerr << "[+] Display application running." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}