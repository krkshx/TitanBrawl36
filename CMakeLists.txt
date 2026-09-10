cmake_minimum_required(VERSION 3.21)

project(TitanBrawl36 VERSION 36.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ---------------------------------------------------------------
option(TITAN_BUILD_TESTS "Build self-check tests" ON)
option(TITAN_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(TITAN_WITH_SODIUM "Enable libsodium session crypto" OFF)
option(TITAN_BUILD_VIEWER "Build the Qt home-data viewer" OFF)

if(TITAN_WARNINGS_AS_ERRORS)
  if(MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(-Wall -Wextra -Werror)
  endif()
endif()

# ---- Library ---------------------------------------------------------------
# NOTE: titan/gen/* is produced by tools/generate_*.py. Re-run the generators
# after inventory updates. Per-class sources (messages/commands/game/sc) are
# globbed: one file per class, see docs/REVERSE_STATUS.md.
# Messages/commands/game live in titan/<module>/<family>/*.cpp (CPP-only).
file(GLOB_RECURSE TITAN_MSG_SRC CONFIGURE_DEPENDS titan/messages/*.cpp)
file(GLOB_RECURSE TITAN_CMD_SRC CONFIGURE_DEPENDS titan/commands/*.cpp)
file(GLOB_RECURSE TITAN_GAME_SRC CONFIGURE_DEPENDS titan/game/*.cpp)
file(GLOB TITAN_SC_SRC CONFIGURE_DEPENDS titan/sc/*.cpp)
add_library(titan_core STATIC
  titan/core/ChecksumEncoder.cpp
  titan/core/ByteStream.cpp
  titan/core/BitStream.cpp
  titan/core/PiranhaMessage.cpp
  ${TITAN_MSG_SRC}
  ${TITAN_CMD_SRC}
  ${TITAN_GAME_SRC}
  ${TITAN_SC_SRC}
  titan/crypto/Encrypter.cpp
  titan/gen/FunctionRegistry.cpp
  titan/gen/MessageFactory.cpp
  titan/gen/LogicCommands.cpp
  titan/gen/Registry.cpp
  titan/gen/RegistryPart0.cpp
  titan/gen/RegistryPart1.cpp
  titan/gen/RegistryPart2.cpp
  titan/gen/RegistryPart3.cpp
)

target_include_directories(titan_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

if(MSVC)
  target_compile_options(titan_core PRIVATE /W4)
else()
  target_compile_options(titan_core PRIVATE -Wall -Wextra)
endif()

if(TITAN_WITH_SODIUM)
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(SODIUM libsodium)
  endif()
  if(SODIUM_FOUND)
    target_include_directories(titan_core PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(titan_core PUBLIC ${SODIUM_LIBRARIES})
    target_compile_definitions(titan_core PUBLIC TITAN_WITH_SODIUM)
  else()
    find_library(SODIUM_LIB sodium)
    if(SODIUM_LIB)
      target_link_libraries(titan_core PUBLIC ${SODIUM_LIB})
      target_compile_definitions(titan_core PUBLIC TITAN_WITH_SODIUM)
    else()
      message(WARNING "TITAN_WITH_SODIUM=ON but libsodium not found; crypto disabled")
    endif()
  endif()
endif()

# ---- Tests -----------------------------------------------------------------
if(TITAN_BUILD_TESTS)
  enable_testing()
  add_executable(titan_bytestream_test tests/test_bytestream.cpp)
  target_link_libraries(titan_bytestream_test PRIVATE titan_core)
  add_test(NAME bytestream COMMAND titan_bytestream_test)

  add_executable(titan_messages_test tests/test_messages.cpp)
  target_link_libraries(titan_messages_test PRIVATE titan_core)
  add_test(NAME messages COMMAND titan_messages_test)

  add_executable(titan_registry_test tests/test_registry.cpp)
  target_link_libraries(titan_registry_test PRIVATE titan_core)
  add_test(NAME registry COMMAND titan_registry_test)

  add_executable(titan_msgbatch_test tests/test_msgbatch.cpp)
  target_link_libraries(titan_msgbatch_test PRIVATE titan_core)
  add_test(NAME msgbatch COMMAND titan_msgbatch_test)

  add_executable(titan_factory_test tests/test_factory.cpp)
  target_link_libraries(titan_factory_test PRIVATE titan_core)
  add_test(NAME factory COMMAND titan_factory_test)

  add_executable(titan_bitstream_test tests/test_bitstream.cpp)
  target_link_libraries(titan_bitstream_test PRIVATE titan_core)
  add_test(NAME bitstream COMMAND titan_bitstream_test)

  add_executable(titan_crypto_test tests/test_crypto.cpp)
  target_link_libraries(titan_crypto_test PRIVATE titan_core)
  add_test(NAME crypto COMMAND titan_crypto_test)

  add_executable(titan_datatables_test tests/test_datatables.cpp)
  target_link_libraries(titan_datatables_test PRIVATE titan_core)
  add_test(NAME datatables COMMAND titan_datatables_test ${CMAKE_CURRENT_SOURCE_DIR}/assets)
  set_tests_properties(datatables PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  add_executable(titan_entries_test tests/test_entries.cpp)
  target_link_libraries(titan_entries_test PRIVATE titan_core)
  add_test(NAME entries COMMAND titan_entries_test)

  add_executable(titan_json_test tests/test_json.cpp)
  target_link_libraries(titan_json_test PRIVATE titan_core)
  add_test(NAME json COMMAND titan_json_test)

  add_executable(titan_session_test tests/test_session.cpp)
  target_link_libraries(titan_session_test PRIVATE titan_core)
  add_test(NAME session COMMAND titan_session_test)

  add_executable(titan_gameplay_test tests/test_gameplay.cpp)
  target_link_libraries(titan_gameplay_test PRIVATE titan_core)
  add_test(NAME gameplay COMMAND titan_gameplay_test)

  add_executable(titan_sc_test tests/test_sc.cpp)
  target_link_libraries(titan_sc_test PRIVATE titan_core)
  add_test(NAME sc COMMAND titan_sc_test)
endif()

# ---- Viewer (Qt6, optional) --------------------------------------------------
if(TITAN_BUILD_VIEWER)
  add_subdirectory(viewer)
endif()

# ---- Install (keeps all platforms happy) -----------------------------------
install(DIRECTORY titan/ DESTINATION include/titan)
install(TARGETS titan_core ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
