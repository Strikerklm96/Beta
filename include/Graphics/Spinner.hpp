#ifndef SPINNER_HPP
#define SPINNER_HPP

#include "QuadComponent.hpp"

struct SpinnerData : public QuadComponentData
{
    SpinnerData() :
        rate(45),//degrees CCW/s
        randomRot(true)
    {

    }
    float rate;//degrees CCW/s
    bool randomRot;//will we spawn with a random rotation?

};


class Spinner : public QuadComponent
{
public:
    Spinner(const SpinnerData& rData);
    virtual ~Spinner();

    void setRotation(float radiansCCW);//set our rotation in radians CCW (absolute)

    void setRotationRate(float rate);//degrees CCW per second

protected:
private:

    float m_extraRotOffset;//extra offset we have from our rotation rate; radsCCW

    float m_rate;// radiansCCW/s
    Timer m_timer;//tells us how long since last update
};

#endif // SPINNER_HPP
