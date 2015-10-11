#include "snowparticles.h"
Pool<SnowFlake *> *SnowFlake::pool = new Pool<SnowFlake*>([] () {return new SnowFlake();});

SnowParticles::SnowParticles(int width, int height, QImage *image)
{
    this->width = image->width();
    this->height = image->height();
    snow = new float*[width];
    for (int i = 0; i < width; ++i) {
        snow[i] = new float[height];
    }
    this->image = image;

    for (int i = 0; i < 1000; ++i) {
        snowFlakes.push_back(createSnowFlake(SnowFlake::pool->obtain()));
    }

}

void SnowParticles::update(float delta)
{
//    snowFlakes.push_back(createSnowFlake());
    std::cout << delta << std::endl;
    for (int i = 0; i < snowFlakes.size(); ++i) {
        SnowFlake *s = snowFlakes[i];
        s->z -= s->speed * delta;
        int x = s->x * image->width() + image->width() * 0.5;
        int y = s->y * image->width() + image->width() * 0.5;
        float value = qGray(image->pixel(x, y)) * 0.0008f;
        if(value > s->z) {
            snow[x][y] += 0.0001;
            createSnowFlake(s);
        }
    }
}

void SnowParticles::draw(float delta)
{
    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(0.9, 0.8, 0.9);
    for (int i = 0; i < snowFlakes.size(); ++i) {
        SnowFlake *s = snowFlakes[i];
//        int x = s->x * this->image->width();
//        int y = s->y * this->image->height();
        glVertex3f(s->x, s->y, s->z);
    }

//    glBegin();
    for (int i = 0; i < image->width(); ++i) {
        for (int j = 0; j < image->height(); ++j) {
            if(snow[i][j] > 0)
            glVertex3f((float) i / (float)image->width() - 0.5f,
                       (float) j / (float)image->height() - 0.5f,
                       qGray(image->pixel(i, j)) * 0.0008f + snow[i][j]);
        }
    }
    glEnd();
}

SnowFlake *SnowParticles::createSnowFlake(SnowFlake *s)
{
    s->x = (float) ((qrand() % width) - width * 0.5) / ((float) width);
    s->y = (float) ((qrand() % height) - height * 0.5) / (float) height;
    s->z = (qrand() % 100) * 0.01f;
    return s;
}

SnowFlake *SnowFlake::init(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

SnowFlake::SnowFlake()
{
    this->speed = (float (qrand() % 10) + 5.0f) * 0.06f;
}
