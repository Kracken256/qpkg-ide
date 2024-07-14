cmake_minimum_required(VERSION 3.15)
project(qpkg-ide-display CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEBUG_FLAGS -O2 -g3 -Wall -Wextra -Wpedantic)
set(LD_DEBUG_FLAGS )
set(LIBS_DEBUG SDL2 SDL2_gfx)

set(TEST_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic)
set(LD_TEST_FLAGS -flto)
set(LIBS_TEST SDL2 SDL2_gfx)

set(TRACE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -fprofile-arcs -ftest-coverage)
set(LD_TRACE_FLAGS -fprofile-arcs -ftest-coverage)
set(LIBS_TRACE SDL2 SDL2_gfx)

set(PRERELEASE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_PRERELEASE_FLAGS -static-libstdc++ -static-libgcc)
set(LIBS_PRERELEASE SDL2 SDL2_gfx)

set(RELEASE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_RELEASE_FLAGS )
set(LIBS_RELEASE SDL2 SDL2_gfx)

file(GLOB_RECURSE SOURCES "src/*.cc" "platform/*.cc")

add_library(qpkg-ide-display STATIC ${SOURCES})
target_include_directories(qpkg-ide-display PRIVATE "src" "include"
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-platform/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-core/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-business/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-utility/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(qpkg-ide-display PRIVATE ${DEBUG_FLAGS})
  target_link_options(qpkg-ide-display PRIVATE ${LD_DEBUG_FLAGS})
  target_link_libraries(qpkg-ide-display PRIVATE ${LIBS_DEBUG})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
  target_compile_options(qpkg-ide-display PRIVATE ${TEST_FLAGS})
  target_link_options(qpkg-ide-display PRIVATE ${LD_TEST_FLAGS})
  target_link_libraries(qpkg-ide-display PRIVATE ${LIBS_TEST})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Trace")
  target_compile_options(qpkg-ide-display PRIVATE ${TRACE_FLAGS})
  target_link_options(qpkg-ide-display PRIVATE ${LD_TRACE_FLAGS})
  target_link_libraries(qpkg-ide-display PRIVATE ${LIBS_TRACE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "PreRelease")
  target_compile_options(qpkg-ide-display PRIVATE ${PRERELEASE_FLAGS})
  target_link_options(qpkg-ide-display PRIVATE ${LD_PRERELEASE_FLAGS})
  target_link_libraries(qpkg-ide-display PRIVATE ${LIBS_PRERELEASE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(qpkg-ide-display PRIVATE ${RELEASE_FLAGS})
  target_link_options(qpkg-ide-display PRIVATE ${LD_RELEASE_FLAGS})
  target_link_libraries(qpkg-ide-display PRIVATE ${LIBS_RELEASE})
endif()