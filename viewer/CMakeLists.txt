# Titan home-data viewer (Qt6 Widgets, optional).
# Enable:  cmake -DTITAN_BUILD_VIEWER=ON ..
# Requires Qt6 Widgets; core library stays dependency-free.

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)

add_executable(titan_viewer
  main.cpp
  HomeViewer.cpp
)
target_link_libraries(titan_viewer PRIVATE titan_core Qt6::Widgets)
target_include_directories(titan_viewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
