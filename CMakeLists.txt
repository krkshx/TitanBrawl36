cmake_minimum_required(VERSION 3.21)

project(TitanBrawl36 VERSION 36.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ---------------------------------------------------------------
option(TITAN_BUILD_TESTS "Build self-check tests" ON)
option(TITAN_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

if(TITAN_WARNINGS_AS_ERRORS)
  if(MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(-Wall -Wextra -Werror)
  endif()
endif()

# ---- Library ---------------------------------------------------------------
# NOTE: src/gen/Registry*.cpp is produced by tools/generate_registry.py
# from data/ida_shard_*.csv. Re-run the generator after inventory updates.
add_library(titan_core STATIC
  src/core/ChecksumEncoder.cpp
  src/core/ByteStream.cpp
  src/core/PiranhaMessage.cpp
  src/messages/ResetAccountMessage.cpp
  src/messages/KeepAliveMessages.cpp
  src/gen/Registry.cpp
  src/gen/RegistryPart0.cpp
  src/gen/RegistryPart1.cpp
  src/gen/RegistryPart2.cpp
  src/gen/RegistryPart3.cpp
)

target_include_directories(titan_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(MSVC)
  target_compile_options(titan_core PRIVATE /W4)
else()
  target_compile_options(titan_core PRIVATE -Wall -Wextra)
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
endif()

# ---- Install (keeps all platforms happy) -----------------------------------
install(DIRECTORY include/ DESTINATION include)
install(TARGETS titan_core ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
