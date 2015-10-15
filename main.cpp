#include "gamewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QMainWindow>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include "camera.h"
#include "serverthread.h"
using namespace std;

#define SERVER 1
#define CLIENT 0

#define SUMMER "summer"
#define AUTUMN "autumn"
#define WINTER "winter"
#define SPRING "spring"

class ControllerWindow : public QMainWindow {
public:
    ControllerWindow()
    {
        server = new ServerThread();
        button = new QPushButton();
        this->setCentralWidget(button);
        button->setText("Change season");
        button->resize(100, 60);
        connect(button, SIGNAL(clicked()), server, SLOT(onSeasonChangeRequest()));
    }

private:
    ServerThread *server;
    QPushButton *button;
};

GameWindow *createWindow(Camera* camera, float framerate) {
    QSurfaceFormat format;
    format.setSamples(16);

    GameWindow *w = new GameWindow(camera, framerate);
    w->setFormat(format);
    w->resize(640, 480);
    w->show();
    w->setAnimating(true);
    return w;
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    QApplication app(argc, argv);

    ControllerWindow window;
    window.show();
    window.resize(200, 120);
    window.move(1350, 400);

    Camera *c = new Camera();
    GameWindow *g = createWindow(c, 1.0f / 60.0f);
    g->setPosition(120, 10);
    g = createWindow(c, 1.0f / 60.0f);
    g->setPosition(760, 10);
    g = createWindow(c, 1.0f / 60.0f);
    g->setPosition(760, 490);
    g = createWindow(c, 1.0f / 60.0f);
    g->setPosition(120, 490);

    return app.exec();
}



