cmake_minimum_required(VERSION 3.10)

project(FoodIdeaMobile VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.14 REQUIRED COMPONENTS Quick QuickControls2)

add_executable(FoodIdeaMobile
    main.cpp
    foodcontroller.cpp
    foodcontroller.h
    dish.cpp
    dish.h
    databasemanager.cpp
    databasemanager.h
    qml.qrc
)

target_link_libraries(FoodIdeaMobile PRIVATE
    Qt5::Quick
    Qt5::QuickControls2
)

if(WIN32)
    set_target_properties(FoodIdeaMobile PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

install(TARGETS FoodIdeaMobile
    RUNTIME DESTINATION bin
)
