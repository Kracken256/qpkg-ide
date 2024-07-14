#include <iostream>
#include <qpkg-ide-business/Library.hh>
#include <qpkg-ide-core/Library.hh>
#include <qpkg-ide-display/Library.hh>
#include <qpkg-ide-platform/Library.hh>
#include <qpkg-ide-utility/Library.hh>
#include <string_view>
#include <vector>

using namespace qpkg::ide;
using std::cerr;

int main(int argc, char *argv[]) {
  std::vector<std::string_view> args(argv, argv + argc);

  for (const auto &arg : args) {
    std::cout << arg << std::endl;
  }

  if (!platform::init_library()) {
    cerr << "CRITICAL: Failed to initialize platform library.\n";
    cerr << "qpkg::ide::platform::init_library() returned false. Exiting...\n";
    return 1;
  }

  if (!core::init_library()) {
    cerr << "CRITICAL: Failed to initialize core library.\n";
    cerr << "qpkg::ide::core::init_library() returned false. Exiting...\n";
    return 2;
  }

  if (!business::init_library()) {
    cerr << "CRITICAL: Failed to initialize business library.\n";
    cerr << "qpkg::ide::business::init_library() returned false. Exiting...\n";
    return 3;
  }

  if (!utility::init_library()) {
    cerr << "CRITICAL: Failed to initialize utility library.\n";
    cerr << "qpkg::ide::utility::init_library() returned false. Exiting...\n";
    return 4;
  }

  if (!display::init_library()) {
    cerr << "CRITICAL: Failed to initialize display library.\n";
    cerr << "qpkg::ide::display::init_library() returned false. Exiting...\n";
    return 5;
  }

  /// TODO: Call into the library function; starting the application.

  std::cout << "Simulation of IDE application.\n";

  if (!display::deinit_library()) {
    cerr << "CRITICAL: Failed to deinitialize display library.\n";
    cerr << "qpkg::ide::display::deinit_library() returned false. Exiting...\n";
    return 6;
  }

  if (!utility::deinit_library()) {
    cerr << "CRITICAL: Failed to deinitialize utility library.\n";
    cerr << "qpkg::ide::utility::deinit_library() returned false. Exiting...\n";
    return 7;
  }

  if (!business::deinit_library()) {
    cerr << "CRITICAL: Failed to deinitialize business library.\n";
    cerr << "qpkg::ide::business::deinit_library() returned false. Exiting...\n";
    return 8;
  }

  if (!core::deinit_library()) {
    cerr << "CRITICAL: Failed to deinitialize core library.\n";
    cerr << "qpkg::ide::core::deinit_library() returned false. Exiting...\n";
    return 9;
  }

  if (!platform::deinit_library()) {
    cerr << "CRITICAL: Failed to deinitialize platform library.\n";
    cerr << "qpkg::ide::platform::deinit_library() returned false. Exiting...\n";
    return 10;
  }

  std::cout << "IDE application has been successfully terminated.\n";

  return 0;
}
