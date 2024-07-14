#include <cassert>
#include <iostream>
#include <qpkg-ide-core/Layer.hh>

#include <filesystem>
#include <fstream>

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

bool qpkg::ide::core::ICoreLayer::request_resource(std::string_view resource_id,
                                                   qpkg::ide::core::ResourceType type,
                                                   std::string &out) {
  std::filesystem::path appdir = m_platform_api->app_install_dir();
  std::filesystem::path resource_path = appdir / "resources";

  switch (type) {
  case ResourceType::JsonData:
    resource_path /= "json";
    resource_path /= resource_id;
    break;
  case ResourceType::BinaryData:
    resource_path /= "bin";
    resource_path /= resource_id;
    break;
  case ResourceType::MediaData:
    resource_path /= "media";
    resource_path /= resource_id;
    break;
  }

  try {
    std::ifstream file(resource_path);
    if (!file.is_open()) {
      std::cerr << "Failed to open resource file: " << resource_path << std::endl;
      return false;
    }

    out = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true;
  } catch (const std::exception &e) {
    std::cerr << "Failed to read resource file: " << resource_path << std::endl;
    return false;
  }
}