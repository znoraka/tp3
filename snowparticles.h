#ifndef SNOWPARTICLES_H
#define SNOWPARTICLES_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QtGui/QOpenGLFunctions>
#include <iostream>

#include "pool.cpp"

class SnowFlake {
    friend class Pool<SnowFlake *>;

public:
    float x, y, z, speed;
    static Pool<SnowFlake *> *pool;
    SnowFlake *init(float x, float y, float z);
private:
    SnowFlake();
};

class SnowParticles
{
public:
    SnowParticles(int width, int height, QImage *image);
    void update(float delta);
    void draw(float delta);
    void reset();
private:
    int width;
    int height;
    float **snow;
    QImage *image;
    QVector<SnowFlake*> snowFlakes;

    SnowFlake *createSnowFlake(SnowFlake *s);
};

#endif // SNOWPARTICLES_H
