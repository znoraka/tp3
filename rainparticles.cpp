#include "rainparticles.h"
Pool<RainDrop *> *RainDrop::pool = new Pool<RainDrop*>([] () {return new RainDrop();});
GLfloat RainParticles::lightPos[4] = {1.0, 0.0, -0.7, 1.0};

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
        glColor4f(0.2, 0.2, 1, 1);
        for (int i = 0; i < rainDrops.size(); ++i) {
            RainDrop *s = rainDrops[i];
            glVertex3f(s->x, s->y, s->z);
            glVertex3f(s->x, s->y, s->z + 0.01);
        }
        glEnd();
    }

    if(waterHeight > 0.000) {
        glColor4f(0.2, 0.2, 1, 0.5);
        glBegin(GL_QUADS);
        for (int i = -50; i < 50; ++i) {
            for (int j = -50; j < 50; ++j) {
                float r = 0;
                point p1, p2, p3, p4;
                p1.x = i * 0.01; p1.y = j * 0.01;
                p1.z = waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005;

                p2.x = (i + 1) * 0.01; p2.y = j * 0.01;
                p2.z = waterHeight + sin(i + 1 + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005;

                p3.x = (i + 1) * 0.01; p3.y = (j + 1) * 0.01;
                p3.z = waterHeight + sin(i + 1 + elapsed + r) * 0.005 + cos(j + 1+ elapsed + r) * 0.005;

                p4.x = i * 0.01; p4.y = (j + 1) * 0.01;
                p4.z = waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + 1 + elapsed + r) * 0.005;

                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
                glVertex3f(p3.x, p3.y, p3.z);
                glVertex3f(p4.x, p4.y, p4.z);

                std::vector<float> vec = Utils::getNormal(p1, p3, p2);
                glNormal3f(vec[0], vec[1], vec[2]);
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
