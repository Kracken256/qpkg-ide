#include <cstring>
#include <iostream>
#include <qpkg-ide-business/Layer.hh>
#include <qpkg-ide-core/Layer.hh>
#include <qpkg-ide-display/Layer.hh>
#include <qpkg-ide-platform/Layer.hh>
#include <qpkg-ide-utility/Layer.hh>
#include <stdexcept>
#include <string_view>
#include <vector>

using namespace qpkg::ide;

static int hash_error(const char *msg) {
  int hash = strlen(msg) % 255;

  if (hash == 0) {
    hash = 1;
  }

  return hash;
}

int main(int argc, char *argv[]) {
  std::vector<std::string_view> args(argv, argv + argc);

  try {
    auto platform = std::make_unique<platform::IPlatformLayer>(args);
    auto core = std::make_unique<core::ICoreLayer>(std::move(platform));
    auto business = std::make_unique<business::IBusinessLayer>(std::move(core));
    auto utility = std::make_unique<utility::IUtilityLayer>(std::move(business));
    auto display = std::make_unique<display::IDisplayLayer>(std::move(utility));

    display->start_application();
  } catch (const std::exception &e) {
    std::cerr << "CRITICAL: " << e.what() << '\n';
    return hash_error(e.what());
  }

  std::cout << "IDE application has been successfully terminated.\n";

  return 0;
}
