cmake_minimum_required(VERSION 3.14)

project(RandomPointsInCircle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    main.cpp
    ui/mainwindow.cpp
    logic/algorithm_management.cpp
    utils/mouse_management.cpp
    utils/basic_methods.cpp
    utils/helper_methods.cpp
)

set(HEADERS
    ui/mainwindow.h
)

foreach(FILE ${SOURCES} ${HEADERS})
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/${FILE})
        file(WRITE ${CMAKE_SOURCE_DIR}/${FILE})
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)
