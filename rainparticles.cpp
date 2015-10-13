#include "rainparticles.h"
Pool<RainDrop *> *RainDrop::pool = new Pool<RainDrop*>([] () {return new RainDrop();});

RainParticles::RainParticles(int width, int height, QImage *image)
{
    this->width = image->width();
    this->height = image->height();
    this->image = image;
    this->waterHeight = 0.0;
    this->elapsed = 0;
    this->isActive = false;
}

void RainParticles::update(float delta)
{
    if(isActive) {
        if(rainDrops.size() < 1000 && qrand() % 100 < 25) {
            rainDrops.push_back(createRainDrops(RainDrop::pool->obtain()));
        }

        for (int i = 0; i < rainDrops.size(); ++i) {
            RainDrop *s = rainDrops[i];
            s->z -= s->speed * delta;
            int x = s->x * image->width() + image->width() * 0.5;
            int y = s->y * image->width() + image->width() * 0.5;
            float value = qGray(image->pixel(x, y)) * 0.0008f;
            if(value > s->z) {
                createRainDrops(s);
                waterHeight += 0.0000005f;
            }
        }
    } else {
        if(waterHeight > 0.001)
            waterHeight -= 0.01 * delta;
    }
}

void RainParticles::draw(float delta)
{
    elapsed += delta * 10;
    if(isActive) {
        glPointSize(1);
        glBegin(GL_LINES);
        glColor4f(0.2, 0.2, 1, 0.5);
        for (int i = 0; i < rainDrops.size(); ++i) {
            RainDrop *s = rainDrops[i];
            glVertex3f(s->x, s->y, s->z);
            glVertex3f(s->x, s->y, s->z + 0.01);
        }
        glEnd();
    }

    if(waterHeight > 0.0001) {

        glColor4f(0.2, 0.2, 1, 0.1);
        glBegin(GL_QUADS);
        for (int i = -50; i < 50; ++i) {
            for (int j = -50; j < 50; ++j) {
                //            qDebug() << i * 0.01 << ", " << j * 0.01;
                float r = 0;
                glVertex3f(i * 0.01, j * 0.01, waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005);
                glVertex3f((i + 1) * 0.01, j * 0.01, waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005);
                glVertex3f((i + 1) * 0.01, (j + 1) * 0.01, waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005);
                glVertex3f(i * 0.01, (j + 1) * 0.01, waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005);
            }
        }
        glEnd();
    }
}

void RainParticles::reset()
{
    for (int i = 0; i < rainDrops.size(); ++i) {
        RainDrop::pool->release(rainDrops[i]);
    }
    rainDrops.clear();
//    waterHeight = 0;
}

void RainParticles::setActive(bool active)
{
    this->isActive = active;
}

RainDrop *RainParticles::createRainDrops(RainDrop *s)
{
    s->x = (float) ((qrand() % width) - width * 0.5) / ((float) width);
    s->y = (float) ((qrand() % height) - height * 0.5) / (float) height;
    s->z = (qrand() % 100) * 0.01f;
    return s;
}

RainDrop *RainDrop::init(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

RainDrop::RainDrop()
{
    this->speed = (float (qrand() % 10) + 50.0f) * 0.06f;
}
