QT += core widgets charts serialport

CONFIG += c++17

TARGET = SatelliteApp
TEMPLATE = app

# 源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    nmeaview.cpp \
    basicview.cpp \
    messageview.cpp \
    satelliteview.cpp \
    snrview.cpp \
    nmeaparser.cpp \
    satellitedata.cpp \
    filemanager.cpp \
    serialmanager.cpp \
    chartmanager.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    nmeaview.h \
    basicview.h \
    messageview.h \
    satelliteview.h \
    snrview.h \
    nmeaparser.h \
    satellitedata.h \
    filemanager.h \
    serialmanager.h \
    chartmanager.h \
    gnssdata.h

# UI文件
FORMS += \
    mainwindow.ui

# 资源文件
RESOURCES += resources.qrc

# 编译配置
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    RCC_DIR = debug/rcc
    UI_DIR = debug/ui
}

CONFIG(release, debug|release) {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/obj
    MOC_DIR = release/moc
    RCC_DIR = release/rcc
    UI_DIR = release/ui
}
