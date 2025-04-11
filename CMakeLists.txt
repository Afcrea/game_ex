cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME game_ex)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "game")
set(WINDOW_WIDTH 640)
set(WINDOW_HEIGHT 480)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/scene)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/ui_customize.cpp src/ui_customize.cpp
    src/application.h src/application.cpp

    scene/scene.h scene/scene_manager.h scene/scene_manager.cpp
    scene/lobby_scene.h scene/lobby_scene.cpp
    scene/main_scene.h scene/main_scene.cpp
    )

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
WINDOW_NAME="${WINDOW_NAME}"
WINDOW_WIDTH=${WINDOW_WIDTH}
WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})