cmake_minimum_required(VERSION 3.15)
project(qpkg-ide-business CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEBUG_FLAGS -O2 -g3 -Wall -Wextra -Wpedantic)
set(LD_DEBUG_FLAGS )
set(LIBS_DEBUG )

set(TEST_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic)
set(LD_TEST_FLAGS -flto)
set(LIBS_TEST)

set(TRACE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -fprofile-arcs -ftest-coverage)
set(LD_TRACE_FLAGS -fprofile-arcs -ftest-coverage)
set(LIBS_TRACE )

set(PRERELEASE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_PRERELEASE_FLAGS -static-libstdc++ -static-libgcc)
set(LIBS_PRERELEASE )

set(RELEASE_FLAGS -O3 -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_RELEASE_FLAGS )
set(LIBS_RELEASE )

file(GLOB_RECURSE SOURCES "src/*.cc" "platform/*.cc")

add_library(qpkg-ide-business STATIC ${SOURCES})
target_include_directories(qpkg-ide-business PRIVATE "src" "include"
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-platform/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-core/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(qpkg-ide-business PRIVATE ${DEBUG_FLAGS})
  target_link_options(qpkg-ide-business PRIVATE ${LD_DEBUG_FLAGS})
  target_link_libraries(qpkg-ide-business PRIVATE ${LIBS_DEBUG})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
  target_compile_options(qpkg-ide-business PRIVATE ${TEST_FLAGS})
  target_link_options(qpkg-ide-business PRIVATE ${LD_TEST_FLAGS})
  target_link_libraries(qpkg-ide-business PRIVATE ${LIBS_TEST})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Trace")
  target_compile_options(qpkg-ide-business PRIVATE ${TRACE_FLAGS})
  target_link_options(qpkg-ide-business PRIVATE ${LD_TRACE_FLAGS})
  target_link_libraries(qpkg-ide-business PRIVATE ${LIBS_TRACE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "PreRelease")
  target_compile_options(qpkg-ide-business PRIVATE ${PRERELEASE_FLAGS})
  target_link_options(qpkg-ide-business PRIVATE ${LD_PRERELEASE_FLAGS})
  target_link_libraries(qpkg-ide-business PRIVATE ${LIBS_PRERELEASE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(qpkg-ide-business PRIVATE ${RELEASE_FLAGS})
  target_link_options(qpkg-ide-business PRIVATE ${LD_RELEASE_FLAGS})
  target_link_libraries(qpkg-ide-business PRIVATE ${LIBS_RELEASE})
endif()