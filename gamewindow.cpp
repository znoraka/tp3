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

static int firstSeason = 0;
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
}

void GameWindow::initialize()
{
    timer.setInterval(framerate * 1000);
    this->camera->initialize(devicePixelRatio(), width(), height(), 0, 100.0);
    timer.start();
    this->connect(&timer, SIGNAL(timeout()), this, SLOT(renderNow()));

    this->m_image = QImage(":/heightmap-1.png");
    qDebug() << this->m_image.width();

    this->vertices = initVertices(this->m_image.width(), this->m_image.height());

    this->cursor = new QCursor(Qt::BlankCursor);
    this->setCursor(*cursor);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    snow = new SnowParticles(1000, 1000, &this->m_image);
    rain = new RainParticles(0, 0, &this->m_image);
    drought = new Drought();
    spring = new Spring(&this->m_image);
    this->season = firstSeason++;
}

void GameWindow::onSeasonChange()
{
    //TODO changer la saison
    snow->reset();
    rain->reset();
    if(++season >= 4) season = 0;
    qDebug() << season;

}

void GameWindow::render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->render((float) timer.interval() * 0.001f);
}

void GameWindow::render(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(cursorCaptured) {
        this->cursor->setPos(this->position().x() + width() * 0.5f, this->position().y() + height() * 0.5f);
    }
    this->camera->update(delta);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    drawTriangles();
    if(season == 0) {
        snow->setActive(true);
        rain->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 3) {
        rain->setActive(true);
        snow->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 2){
        drought->setActive(true);
        rain->setActive(false);
        snow->setActive(false);
        spring->setActive(false);
    } else {
        spring->setActive(true);
        drought->setActive(false);
        rain->setActive(false);
        snow->setActive(false);
    }
    snow->update(delta);
    snow->draw(delta);
    rain->update(delta);
    rain->draw(delta);
    drought->update(delta);
    spring->update(delta);
    spring->draw(delta);
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
        if(cursorCaptured) {
            mouseEvent = static_cast<QMouseEvent*>(event);
            camera->rotate(-(deltaY - mouseEvent->y()) * 0.1f,
                           0,
                           -(deltaX - mouseEvent->x()) * 0.1f
                           );
        }
        return true;
    case QEvent::MouseButtonPress:
        cursorCaptured = true;
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
        cursorCaptured = false;
        //        qApp->exit();
        break;
    case Qt::Key_Tab:
        qApp->exit();
        break;
    case 'Z':
        camera->scale(0.10f, 0.10f, 0);
        break;
    case 'S':
        camera->scale(-0.10f, -0.10f, 0);
        break;
    case Qt::Key_Up:
        camera->translate(.01, 0, 0);
        break;
    case Qt::Key_Down:
        camera->translate(-.01, 0, 0);
        break;
    case Qt::Key_Left:
        camera->translate(0, .01, 0);
        break;
    case Qt::Key_Right:
        camera->translate(0, -.01, 0);
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

    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

    int countX = m_image.width();
    int countY = m_image.height();
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    glBegin(GL_TRIANGLE_STRIP);
    #pragma omp for schedule(dynamic)
    for (int var = 0; var < count - 9; var += 3) {
        if(vertices[var + 2] < 0.08) {
            glColor3f(vertices[var + 2] + drought->getYellow(), 0.4, 0);
        } else if (vertices[var + 2] > 0.08 && vertices[var + 2] < 0.15 + drought->getSnowHeightModifier()) {
            glColor3f(0.54, 0.27 + vertices[var + 2], 0.07);
        } else {
            glColor3f(0.9, 0.8, 0.9);
        }
        glVertex3f(vertices[var], vertices[var + 1], vertices[var + 2]);
        glNormal3f(normals[var / 3]->x, normals[var / 3]->y, normals[var / 3]->z);
    }
    glEnd();
}

GLfloat *GameWindow::initVertices(GLint countX, GLint countY)
{
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    GLfloat *array = new GLfloat[count];
    GLfloat stepX = 1.0 / (countX);
    GLfloat stepY = 1.0 / (countY);
    int cpt = 0;

    float posX = -0.5f;
    float posY = -0.5f;

    int flop = 1;
    #pragma omp for schedule(dynamic)
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
    #pragma omp for schedule(dynamic)
    for (int var = 0; var < cpt - 9; var += 3) {
        point p1, p2, p3;
        p1.x = array[var+0]; p1.y = array[var+1]; p1.z = array[var+2];
        p2.x = array[var+3]; p2.y = array[var+4]; p2.z = array[var+5];
        p3.x = array[var+6]; p3.y = array[var+7]; p3.z = array[var+8];
        std::vector<float> n;
        if(p1.x == p2.x && p1.y < p2.y) {
            n = Utils::getNormal(p1, p2, p3);
        } else if(p1.x != p2.x && p1.y > p2.y) {
            n = Utils::getNormal(p1, p3, p2);
        } else if(p1.x == p2.x && p1.y > p2.y){
            n = Utils::getNormal(p1, p3, p2);
        } else {
            n = Utils::getNormal(p1, p2, p3);
        }

        point *p = new point(); p->x = n[0]; p->y = n[1]; p->z = n[2];
        normals.push_back(p);
    }
    return array;
}

float GameWindow::getRandomZ(float i, float j)
{
    return qGray(this->m_image.pixel((this->m_image.width() * (i + 0.5f)), (this->m_image.height() * (j + 0.5f)))) * 0.0008f;
}
