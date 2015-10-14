#include "drought.h"
GLfloat Drought::lightPos[4] = {0.0, 0.0, -1, 1.0};

Drought::Drought()
{
    yellow = 0;
    snowHeight = 0;
}

void Drought::update(float delta)
{
    if(isActive) {
        if(yellow < 0.3f) yellow += 0.02f * delta;
        if(snowHeight < 0.05f) snowHeight += 0.002f * delta;
    } else {
        if(yellow > 0) yellow -= 0.02f * delta;
        if(snowHeight > 0) snowHeight -= 0.004f * delta;
    }
}

float Drought::getYellow() const
{
    return yellow;
}

float Drought::getSnowHeightModifier() const
{
    return snowHeight;
}

void Drought::setActive(bool active)
{
    this->isActive = active;
}
