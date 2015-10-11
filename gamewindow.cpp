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
using namespace std;


GameWindow::GameWindow()
{
    camera = new Camera();
}

GameWindow::GameWindow(Camera *camera, float framerate)
{
    this->camera = camera;
    this->framerate = framerate;

    cthread = new ClientThread();
    connect(cthread, SIGNAL(seasonChangeSignal()), this, SLOT(onSeasonChange()));
    //    QtConcurrent::run(QThreadPool::globalInstance(), ClientThread::init, cthread);
}

void GameWindow::initialize()
{

    timer.setInterval(framerate * 1000);
    this->camera->initialize(devicePixelRatio(), width(), height(), 0, 100.0);
    timer.start();
    this->connect(&timer, SIGNAL(timeout()), this, SLOT(renderNow()));

    this->m_image = QImage("/home/noe/Documents/dev/imagina-gmin317-2015/tp1/heightmap-1.png");

    this->vertices = initVertices(this->m_image.width(), this->m_image.height());

    this->cursor = new QCursor(Qt::BlankCursor);
    this->setCursor(*cursor);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    snow = new SnowParticles(1000, 1000, &this->m_image);
    //    loadMap(":/heightmap-2.png");
}

void GameWindow::onSeasonChange()
{
    //TODO changer la saison
    qDebug() << "on season change";
    etat ++;
    if(etat > 5)
        etat = 0;
}

void GameWindow::render()
{
    this->render((float) timer.interval() * 0.001f);
}

void GameWindow::render(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->cursor->setPos(this->position().x() + width() * 0.5f, this->position().y() + height() * 0.5f);

    this->camera->update(delta);

    drawTriangles();
    snow->update(delta);
    snow->draw(delta);
    ++m_frame;
}

bool GameWindow::event(QEvent *event)
{
    QMouseEvent *mouseEvent;
    float deltaX = this->width() * 0.5f;
    float deltaY = this->height() * 0.5f;
    switch (event->type())
    {
    case QEvent::MouseMove:
        mouseEvent = static_cast<QMouseEvent*>(event);
        camera->rotate(-(deltaY - mouseEvent->y()) * 0.1f,
                       0,
                       -(deltaX - mouseEvent->x()) * 0.1f
                       );
        return true;
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        qApp->exit();
        break;
    case 'Z':
        camera->scale(0.10f, 0.10f, 0);
        break;
    case 'S':
        camera->scale(-0.10f, -0.10f, 0);
        break;
    case Qt::Key_Up:
        camera->rotate(1.0f, 0, 0);
        break;
    case Qt::Key_Down:
        camera->rotate(-1.0f, 0, 0);
        break;
    case Qt::Key_Left:
        camera->rotate(0, 0, -1);
        break;
    case Qt::Key_Right:
        camera->rotate(0, 0, 1);
        break;
    case Qt::Key_Space:
        if(fill) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        fill = !fill;
        break;
    case 'W':
        etat ++;
        if(etat > 5)
            etat = 0;
        break;
    case 'C':
        this->camera->setAnimated(!this->camera->isAnimated());
        break;
    case 'P':
        qDebug() << framerate;

        this->framerate *= 2;
        timer.stop();
        timer.start(framerate * 1000);
        break;
    case 'M':
        this->framerate /= 2;
        if(this->framerate < 0.001) this->framerate = 0.001;
        timer.stop();
        timer.start(framerate * 1000);
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

//        loadMap(depth);
        break;
    }
    renderNow();
}

void GameWindow::drawTriangles()
{

    int countX = m_image.width();
    int countY = m_image.height();
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    glBegin(GL_TRIANGLE_STRIP);
    for (int var = 0; var < count; var += 3) {
        if(vertices[var + 2] < 0.08) {
            glColor3f(vertices[var + 2], 0.4, 0);
        } else if (vertices[var + 2] > 0.08 && vertices[var + 2] < 0.15) {
            glColor3f(0.54, 0.27 + vertices[var + 2], 0.07);
        } else {
            glColor3f(0.9, 0.8, 0.9);
        }
        glVertex3f(vertices[var], vertices[var + 1], vertices[var + 2]);
//        std::cout << vertices[var] << ", " << vertices[var+1] << ", " << vertices[var+2] <<std::endl << "******" << std::endl;
    }
    glEnd();
}

GLfloat *GameWindow::initVertices(GLint countX, GLint countY)
{
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    qDebug() << count;
    GLfloat *array = new GLfloat[count];
    GLfloat stepX = 1.0 / (countX);
    GLfloat stepY = 1.0 / (countY);
    int cpt = 0;

    float posX = -0.5f;
    float posY = -0.5f;

    int flop = 1;

    for (int i = 0; i < countX; ++i) {
        for (int j = 0; j < countY; ++j) {
            array[cpt++] = posX;
            array[cpt++] = posY;
            array[cpt++] = getRandomZ(posX, posY);

            array[cpt++] = posX + stepX;
            array[cpt++] = posY;
            array[cpt++] = getRandomZ(posX + stepX, posY);

            posY += stepY * flop;
        }

        array[cpt++] = posX;
        array[cpt++] = posY;
        array[cpt++] = getRandomZ(posX, posY);

        flop *= -1;
        posX += stepX;
    }

    array[cpt++] = posX;
    array[cpt++] = posY;
    array[cpt++] = getRandomZ(posX, posY);
    qDebug() << cpt;
    return array;
}

float GameWindow::getRandomZ(float i, float j)
{
    return qGray(this->m_image.pixel((this->m_image.width() * (i + 0.5f)), (this->m_image.height() * (j + 0.5f)))) * 0.0008f;
}
