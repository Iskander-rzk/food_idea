cmake_minimum_required(VERSION 3.16)
project(FoodRecommender VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(FoodRecommender
    main.cpp
    mainwindow.cpp
    dish.cpp
    databasemanager.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
)

target_link_libraries(FoodRecommender PRIVATE Qt6::Core Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS FoodRecommender
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
