cmake_minimum_required(VERSION 3.5)

project(komunitin_lite_qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5Network)

add_executable(komunitin_lite_qt
  main.cpp
  netservices.cpp
  account.cpp
  transfer.cpp
)
target_link_libraries(komunitin_lite_qt Qt5::Core Qt5::Network)
