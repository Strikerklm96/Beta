#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include "stdafx.hpp"

struct BodyComponentData
{
    BodyComponentData() :
        coords(0,0),
        rotation(0),
        isDynamic(true),
        isBullet(false),
        startAwake(true)
    {

    }
    b2Vec2 coords;
    float rotation;
    bool isDynamic;
    bool isBullet;
    bool startAwake;
};


class BodyComponent
{
public:
    BodyComponent();
    virtual ~BodyComponent();

    const b2Vec2& getPosition() const;

    bool isAwake() const;
    void sleep();
    void wake();
    void wake(b2Vec2 coords, float radiansCCW, b2Vec2 velocity, float angularVel);
    b2Body* getBodyPtr();

protected:
private:
    b2Body* m_pBody;
    b2BodyDef m_bodyDef;

    bool m_awake;
    b2Vec2 m_oldPos;//used for sleep
    float m_oldAngle;//used for sleep
};

#endif // BODYCOMPONENT_HPP
