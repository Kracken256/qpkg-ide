#include <cassert>
#include <iostream>
#include <qpkg-ide-business/Layer.hh>
#include <unordered_map>

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

bool qpkg::ide::business::IBusinessLayer::request_resource(std::string_view resource_name,
                                                           std::string &out) {
  static const std::unordered_map<std::string_view, std::string> intercept_map = {

  };

  if (!resource_name.starts_with("asset://std.")) {
    // Non Standard asset.
    // Not implemented.
    /// TODO:
    std::cerr << "[!] Non-standard asset requested: " << resource_name << std::endl;
    return false;
  }

  resource_name.remove_prefix(12); // Remove "asset://std." prefix.
  std::cerr << "[!] Standard asset requested: " << resource_name << std::endl;

  if (intercept_map.contains(std::string(resource_name))) {
    resource_name = intercept_map.at(resource_name);
  }

  core::ResourceType type;

  if (resource_name.ends_with(".json")) {
    type = core::ResourceType::JsonData;
  } else if (resource_name.ends_with(".bin")) {
    type = core::ResourceType::BinaryData;
  } else {
    type = core::ResourceType::MediaData;
  }

  if (!m_core_api->request_resource(resource_name, type, out)) {
    std::cerr << "[!] Core layer failed to provide resource: " << resource_name << std::endl;
    return false;
  }

  return true;
}
