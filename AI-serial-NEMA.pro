QT += core widgets serialport charts

CONFIG += c++17

TARGET = AI-serial-NEMA
TEMPLATE = app

# 源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serialmanager.cpp \
    nmeaparser.cpp \
    chartmanager.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    serialmanager.h \
    nmeaparser.h \
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
    MOC_DIR = release/moc
    RCC_DIR = release/rcc
    UI_DIR = release/ui
}
