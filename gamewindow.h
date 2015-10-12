#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include <QtConcurrent/QtConcurrent>
#include <QCursor>
#include "QTimer"
#include "camera.h"
#include "clientthread.h"
#include "serverthread.h"
#include "snowparticles.h"
#include "rainparticles.h"

struct point
{
    float x, y ,z;
};



class GameWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GameWindow();
    GameWindow(Camera* camera, float framerate);

    void initialize();
    void render();
    void render(float delta);
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void drawTriangles();
    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);

public slots:
    void onSeasonChange();

private:
    GLfloat *initVertices(GLint countX, GLint county);
    GLfloat getRandomZ(float x, float y);
    std::vector<float> getNormal(point t1, point t2, point t3);

    int m_frame;
    QImage m_image;
    point *p;
    Camera *camera;
    ClientThread *cthread;
    ServerThread *sthread;
    QCursor* cursor;
    GLfloat* vertices;
    SnowParticles *snow;
    RainParticles *rain;
    bool cursorCaptured = false;
    std::vector<point*> normals;

    int carte=1;

    int season = 0;
    int etat = 0;
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float framerate = 1.0f / 60.0f;
    float deltaTime = 0;
    float elapsed = 0;
    float lastUpdate = 0;
    bool fill = false;
    QTimer timer;
};


#endif // GAMEWINDOW_H
