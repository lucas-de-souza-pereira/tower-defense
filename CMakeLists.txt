cmake_minimum_required(VERSION 3.15)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    main.cpp
    src/model/path.cpp
    src/model/enemy.cpp
    src/controller/enemycontroller.cpp
    src/view/enemyview.cpp
    src/controller/towercontroller.cpp  
    src/view/towerview.cpp 
)

add_executable(TowerDefense ${SOURCES})

target_link_libraries(TowerDefense sfml-graphics sfml-window sfml-system)

cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)


add_executable(towerdefense
    src/main.cpp
    src/controller/gamecontroller.cpp
    src/controller/gamestate.cpp
    src/controller/inputhandler.cpp
    src/model/gamelogic.cpp
    src/model/scoreloader.cpp
    src/view/difficultyview.cpp
    src/view/gameplayview.cpp
    src/view/gameview.cpp
    src/view/menuview.cpp
    src/view/scoreview.cpp
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(towerdefense sfml-graphics sfml-window sfml-system)

target_include_directories(towerdefense PRIVATE include)

