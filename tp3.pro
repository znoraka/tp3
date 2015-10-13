INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp \
    camera.cpp \
    clientthread.cpp \
    serverthread.cpp \
    snowparticles.cpp \
    pool.cpp \
    rainparticles.cpp \
    drought.cpp \
    utils.cpp \
    spring.cpp
HEADERS += $$PWD/openglwindow.h \
    gamewindow.h \
    camera.h \
    clientthread.h \
    serverthread.h \
    snowparticles.h \
    pool.h \
    rainparticles.h \
    drought.h \
    utils.h \
    spring.h

SOURCES += \
    main.cpp

target.path = .
INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

QT += network
QT += widgets
CONFIG += c++11

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
