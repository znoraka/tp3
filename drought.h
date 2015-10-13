#ifndef DROUGHT_H
#define DROUGHT_H

#include <QDebug>

class Drought
{
public:
    Drought();
    void update(float delta);
    float getYellow() const;
    float getSnowHeightModifier() const;
    void setActive(bool active);

private:
    float yellow;
    float snowHeight;
    bool isActive;
};

#endif // DROUGHT_H
