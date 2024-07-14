#include <cassert>
#include <iostream>
#include <qpkg-ide-utility/Layer.hh>
#include <rapidjson/document.h>

uint32_t qpkg::ide::utility::IUtilityLayer::ThemeLookupRGBA32(std::string_view name) {
  if (!m_theme_colors.contains(name)) {
    throw std::runtime_error("Theme color item \"" + std::string(name) + "\" not found.");
  }

  auto color = m_theme_colors[name];
  return (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
}

qpkg::ide::utility::RGBA qpkg::ide::utility::IUtilityLayer::ThemeLookupRGBA(std::string_view name) {
  if (!m_theme_colors.contains(name)) {
    throw std::runtime_error("Theme color item \"" + std::string(name) + "\" not found.");
  }

  return m_theme_colors[name];
}

std::optional<qpkg::ide::utility::IUtilityLayer::ThemeData>
qpkg::ide::utility::IUtilityLayer::decode_theme_data(std::string_view json_data) {

  rapidjson::Document doc;
  doc.Parse(json_data.data());
  if (doc.HasParseError()) {
    std::cerr << "[!] Failed to parse theme data: " << doc.GetParseError() << std::endl;
    return std::nullopt;
  }

  ThemeData theme_data;
  if (!doc.HasMember("theme_data")) {
    std::cerr << "[!] Missing theme data member." << std::endl;
    return std::nullopt;
  }
  if (!doc["theme_data"].IsObject()) {
    std::cerr << "[!] Invalid theme data member type." << std::endl;
    return std::nullopt;
  }

  for (const auto &[key, value] : doc["theme_data"].GetObject()) {
    if (!value.IsString()) {
      std::cerr << "[!] Invalid theme data value type." << std::endl;
      return std::nullopt;
    }

    std::string color_str = value.GetString();
    if (color_str.size() != 9 || color_str[0] != '#') {
      std::cerr << "[!] Invalid color string format." << std::endl;
      return std::nullopt;
    }

    RGBA color;
    try {
      color.r = std::stoi(color_str.substr(1, 2), nullptr, 16);
      color.g = std::stoi(color_str.substr(3, 2), nullptr, 16);
      color.b = std::stoi(color_str.substr(5, 2), nullptr, 16);
      color.a = std::stoi(color_str.substr(7, 2), nullptr, 16);
    } catch (const std::exception &e) {
      std::cerr << "[!] Failed to parse color string: " << e.what() << std::endl;
      return std::nullopt;
    }

    theme_data.theme_data[key.GetString()] = color;
  }

  return theme_data;
}

bool qpkg::ide::utility::IUtilityLayer::load_theme_data() {
  /// TODO:
  /// Load theme data from a file or a database.
  std::string json_src;
  // R"(
  //   {
  //     "theme_data": {
  //       "Z0_TOP_BAR_FILL_COLOR": "#404040FF",
  //       "Z0_BOTTOM_BAR_FILL_COLOR": "#101010FF",
  //       "Z0_LSIDEBAR_FILL_COLOR": "#18181BFF",
  //       "Z0_LSIDEBAR_HOVER_FILL_COLOR": "#646464FF",
  //       "Z0_LSIDEBAR_SEP_COLOR": "#404040FF",
  //       "Z0_LSIDEBAR_TAB_HIGH_COLOR": "#4962F2FF"
  //     }
  //   }
  // )";

  if (!m_business_api->request_resource("asset://std.current_theme.json", json_src)) {
    std::cerr << "[!] Failed to load theme data from business layer." << std::endl;
    return false;
  }

  auto theme = decode_theme_data(json_src);
  if (!theme.has_value()) {
    std::cerr << "[!] Failed to load theme data." << std::endl;
    return false;
  }

  m_theme_data_lifetime = std::make_unique<ThemeData>(*theme);

  for (const auto &[key, value] : m_theme_data_lifetime->theme_data) {
    m_theme_colors[key] = value;
  }

  return true;
}