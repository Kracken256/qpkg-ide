#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <qpkg-ide-business/Layer.hh>
#include <string>
#include <string_view>
#include <unordered_map>

namespace qpkg::ide::utility {
  struct RGBA {
    uint8_t r, g, b, a;
  };

  class IUtilityLayer final {
protected:
    static std::atomic<bool> m_initialized;
    
    struct ThemeData {
      std::unordered_map<std::string, RGBA> theme_data;
    };

    std::unique_ptr<business::IBusinessLayer> m_business_api;

    std::unique_ptr<ThemeData> m_theme_data_lifetime;
    std::unordered_map<std::string_view, RGBA> m_theme_colors;
    std::unordered_map<std::string_view, std::string_view> m_asset_paths;

    bool init_layer();
    bool deinit_layer();

    bool load_theme_data();
    bool load_asset_paths();

    static std::optional<ThemeData> decode_theme_data(std::string_view json_data);

public:
    IUtilityLayer(std::unique_ptr<business::IBusinessLayer> business);
    ~IUtilityLayer();

    virtual uint32_t ThemeLookupRGBA32(std::string_view name);
    virtual RGBA ThemeLookupRGBA(std::string_view name);
    virtual bool DoesAssetExist(std::string_view name);
    virtual std::string_view AssetPath(std::string_view name);
  };
} // namespace qpkg::ide::utility
