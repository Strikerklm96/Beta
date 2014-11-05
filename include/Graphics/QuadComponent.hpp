#ifndef QUADCOMPONENT_HPP
#define QUADCOMPONENT_HPP

#include "GraphicsComponent.hpp"


struct QuadComponentData : public GraphicsComponentData
{
    std::string animName = "default.acfg";//name of our animation
};


class QuadComponent : public GraphicsComponent
{
public:
    QuadComponent(const QuadComponentData& rData);
    virtual ~QuadComponent();

    ///void setAnimation();

protected:
private:
};

#endif // QUADCOMPONENT_HPP
