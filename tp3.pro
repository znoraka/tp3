INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp \
    camera.cpp \
    clientthread.cpp \
    serverthread.cpp
HEADERS += $$PWD/openglwindow.h \
    gamewindow.h \
    camera.h \
    clientthread.h \
    serverthread.h

SOURCES += \
    main.cpp

target.path = .
INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

QT += network
CONFIG += c++11
