QT += quick quickcontrols2
CONFIG += c++14
CONFIG += qt warn_on

TEMPLATE = app
TARGET = FoodIdeaMobile

SOURCES += \
    main.cpp \
    foodcontroller.cpp \
    dish.cpp \
    databasemanager.cpp

HEADERS += \
    foodcontroller.h \
    dish.h \
    databasemanager.h

RESOURCES += qml.qrc

# Application identity used by QSettings.
DEFINES += QT_DEPRECATED_WARNINGS

win32:CONFIG(release, debug|release): CONFIG += embed_manifest_exe

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
