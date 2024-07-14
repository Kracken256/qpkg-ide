cmake_minimum_required(VERSION 3.15)
project(qpkg-ide-linux CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEBUG_FLAGS -O2 -g3 -Wall -Wextra -Wpedantic)
set(LD_DEBUG_FLAGS )
set(LIBS_DEBUG qpkg-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)

set(TEST_FLAGS -O3 -flto -g3 -Wall -Wextra -Wpedantic)
set(LD_TEST_FLAGS -flto)
set(LIBS_TEST qpkg-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)

set(TRACE_FLAGS -O3 -flto -g3 -Wall -Wextra -Wpedantic -Werror -fprofile-arcs -ftest-coverage)
set(LD_TRACE_FLAGS -flto -fprofile-arcs -ftest-coverage)
set(LIBS_TRACE qpke-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)

set(PRERELEASE_FLAGS -O3 -flto -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_PRERELEASE_FLAGS -flto -static-libstdc++ -static-libgcc)
set(LIBS_PRERELEASE qpke-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)

set(RELEASE_FLAGS -O3 -flto -g3 -Wall -Wextra -Wpedantic -Werror -DNDEBUG)
set(LD_RELEASE_FLAGS -flto -s -static-libstdc++ -static-libgcc)
set(LIBS_RELEASE qpke-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)

file(GLOB_RECURSE SOURCES "src/*.cc" "platform/*.cc")

add_executable(qpkg-ide-linux ${SOURCES})
target_include_directories(qpkg-ide-linux PRIVATE "src" 
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-platform/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-core/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-business/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-utility/include
  ${CMAKE_SOURCE_DIR}/libs/qpkg-ide-display/include)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(qpkg-ide-linux PRIVATE ${DEBUG_FLAGS})
  target_link_options(qpkg-ide-linux PRIVATE ${LD_DEBUG_FLAGS})
  target_link_libraries(qpkg-ide-linux PRIVATE ${LIBS_DEBUG})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
  target_compile_options(qpkg-ide-linux PRIVATE ${TEST_FLAGS})
  target_link_options(qpkg-ide-linux PRIVATE ${LD_TEST_FLAGS})
  target_link_libraries(qpkg-ide-linux PRIVATE ${LIBS_TEST})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Trace")
  target_compile_options(qpkg-ide-linux PRIVATE ${TRACE_FLAGS})
  target_link_options(qpkg-ide-linux PRIVATE ${LD_TRACE_FLAGS})
  target_link_libraries(qpkg-ide-linux PRIVATE ${LIBS_TRACE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "PreRelease")
  target_compile_options(qpkg-ide-linux PRIVATE ${PRERELEASE_FLAGS})
  target_link_options(qpkg-ide-linux PRIVATE ${LD_PRERELEASE_FLAGS})
  target_link_libraries(qpkg-ide-linux PRIVATE ${LIBS_PRERELEASE})

  add_custom_command(TARGET qpkg-ide-linux POST_BUILD
    COMMAND upx --best $<TARGET_FILE:qpkg-ide-linux>
  )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(qpkg-ide-linux PRIVATE ${RELEASE_FLAGS})
  target_link_options(qpkg-ide-linux PRIVATE ${LD_RELEASE_FLAGS})
  target_link_libraries(qpkg-ide-linux PRIVATE ${LIBS_RELEASE})

  add_custom_command(TARGET qpkg-ide-linux POST_BUILD
    COMMAND upx --best $<TARGET_FILE:qpkg-ide-linux>
  )
endif()


add_dependencies(qpkg-ide-linux qpkg-ide-platform qpkg-ide-core qpkg-ide-business qpkg-ide-utility qpkg-ide-display)
