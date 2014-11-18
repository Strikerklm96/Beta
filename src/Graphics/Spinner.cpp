#include "Spinner.hpp"

#include "Convert.hpp"

Spinner::Spinner(const SpinnerData& rData) : QuadComponent(rData)
{

    m_rate = leon::degToRad(rData.rate);

    if(rData.randomRot)
        m_extraRotOffset = rand()%7;
}
Spinner::~Spinner()
{

}
void Spinner::setRotation(float radiansCCW)
{
    m_extraRotOffset += m_timer.getTimeElapsed()*m_rate;
    QuadComponent::setRotation(radiansCCW+m_extraRotOffset);
}
void Spinner::setRotationRate(float rate)
{
    m_rate = leon::degToRad(rate);
}
