#ifndef RAINPARTICLES_H
#define RAINPARTICLES_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QtGui/QOpenGLFunctions>
#include <iostream>

#include "pool.cpp"

class RainDrop {
    friend class Pool<RainDrop *>;

public:
    float x, y, z, speed;
    static Pool<RainDrop *> *pool;
    RainDrop *init(float x, float y, float z);
private:
    RainDrop();
};

class RainParticles
{
public:
    RainParticles(int width, int height, QImage *image);
    void update(float delta);
    void draw(float delta);
    void reset();
private:
    int width;
    int height;
    float waterHeight;
    QImage *image;
    QVector<RainDrop*> rainDrops;

    RainDrop *createRainDrops(RainDrop *s);
};

#endif // RAINPARTICLES_H
