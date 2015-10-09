#include "gamewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include "camera.h"
using namespace std;

#define SERVER 1
#define CLIENT 0

GameWindow *createWindow(Camera* camera, float framerate, int type) {
    QSurfaceFormat format;
    format.setSamples(16);

    GameWindow *w = new GameWindow(camera, framerate, type);
    w->setFormat(format);
    w->resize(480, 340);
    w->show();
    w->setAnimating(true);
    return w;
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    Camera *c = new Camera();
    GameWindow *g = createWindow(c, 1.0f / 120.0f, SERVER);
    g->setPosition(120, 10);
    g = createWindow(c, 1.0f / 60.0f, CLIENT);
    g->setPosition(600, 10);
    g = createWindow(c, 1.0f / 30.0f, CLIENT);
    g->setPosition(120, 400);
    g = createWindow(c, 1.0f / 1.0f, CLIENT);
    g->setPosition(600, 400);

    return app.exec();
}



