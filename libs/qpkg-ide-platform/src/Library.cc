#include <algorithm>
#include <cassert>
#include <iostream>
#include <qpkg-ide-platform/Layer.hh>

std::vector<std::string_view> g_cli_args;
std::atomic<bool> qpkg::ide::platform::IPlatformLayer::m_initialized;

qpkg::ide::platform::IPlatformLayer::IPlatformLayer(std::vector<std::string_view> args) {
  assert(!m_initialized);

  std::copy(args.begin(), args.end(), std::back_inserter(g_cli_args));

  if (!init_layer()) {
    std::cerr << "Failed to initialize platform layer." << '\n';
  }
}

qpkg::ide::platform::IPlatformLayer::~IPlatformLayer() {
  if (!deinit_layer()) {
    std::cerr << "Failed to deinitialize platform layer." << '\n';
  }
}

bool qpkg::ide::platform::IPlatformLayer::init_layer() {
  /// TODO: Implement platform layer initialization.

  std::cerr << "[+] Platform layer initialized." << '\n';
  m_initialized = true;
  return true;
}

bool qpkg::ide::platform::IPlatformLayer::deinit_layer() {
  /// TODO: Implement platform library deinitialization.

  std::cerr << "[+] Platform layer deinitialized." << '\n';
  m_initialized = false;
  return true;
}

std::string_view qpkg::ide::platform::IPlatformLayer::app_install_dir() const {
#if defined(__linux__) || defined(__APPLE__)
  return "/usr/local/share/qpkg-ide";
#elif defined(_WIN32)
  return "C:\\Program Files\\qpkg-ide";
#else
#error "Unsupported platform."
#endif
}