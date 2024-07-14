#include <cassert>
#include <qpkg-ide-utility/Layer.hh>

bool qpkg::ide::utility::IUtilityLayer::DoesAssetExist(std::string_view name) {
  return m_asset_paths.contains(name);
}

std::string_view qpkg::ide::utility::IUtilityLayer::AssetPath(std::string_view name) {
  if (!m_asset_paths.contains(name)) {
    throw std::runtime_error("Internal asset named \"" + std::string(name) + "\" not found.");
  }

  return m_asset_paths[name];
}

bool qpkg::ide::utility::IUtilityLayer::load_asset_paths() {
  /// TODO:
  /// Load asset paths from a file or a database.

  m_asset_paths["Z0_LSIDEBAR_SOURCEFILE_ICON_BMP"] =
      "/home/wesjo/Downloads/icons8-page-orientation-48.bmp";
  m_asset_paths["Z0_LSIDEBAR_SEARCH_ICON_BMP"] = "/home/wesjo/Downloads/icons8-view-48.bmp";
  m_asset_paths["Z0_LSIDEBAR_GIT_ICON_BMP"] = "/home/wesjo/Downloads/icons8-scroll-48.bmp";
  m_asset_paths["Z0_LSIDEBAR_RUN_AND_DEBUG_ICON_BMP"] =
      "/home/wesjo/Downloads/icons8-fire-exit-48.bmp";
  m_asset_paths["Z0_LSIDEBAR_EXTENSIONS_ICON_BMP"] = "/home/wesjo/Downloads/icons8-usb-2-48.bmp";
  m_asset_paths["Z0_LSIDEBAR_SETTINGS_ICON_BMP"] = "/home/wesjo/Downloads/icons8-support-48.bmp";

  return true;
}
