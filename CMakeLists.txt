cmake_minimum_required(VERSION 3.15)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    main.cpp
    src/model/path.cpp
    src/controller/enemycontroller.cpp
    src/view/enemyview.cpp
)

add_executable(TowerDefense ${SOURCES})

target_link_libraries(TowerDefense sfml-graphics sfml-window sfml-system)
