#ifndef FIXTURECOMPONENT_HPP
#define FIXTURECOMPONENT_HPP

#include "stdafx.hpp"

enum class Shape
{
    Rectangle,
    Circle,
};

struct FixtureComponentData
{
    FixtureComponentData() :
    offset(0,0),
           ///COLLISION MASK AND CATEGORY
           pBody(NULL),
           shape(Shape::Rectangle),
           size(1,1),
           density(1.f),
           friction(0.3f),
           restitution(0.3f),
           isSensor(false)
    {

    }
    b2Vec2 offset;//offset from center of body
    ///COLLISION MASK AND CATEGORY
    b2Body* pBody;//who owns us
    Shape shape;

    b2Vec2 size;//width and height in world coordinates
    float density;
    float friction;
    float restitution;//how bouncy are we?
    bool isSensor;//are we a sensor
};


class FixtureComponent
{
public:
    FixtureComponent(const FixtureComponentData& rData);
    virtual ~FixtureComponent();

    b2Vec2& getCenter() const;
    float getAngle() const;//RADIANS CCW
    int getIOPos() const;

    void applyForce();//applies force to center of body(Newtons)
    void applyForceFixture();//applies force at the center of fixture
    void applyTorque(float radiansCCW);//applies torque to body(Newton Meters)



    void startContact(FixtureComponent* pOther);
    void endContact(FixtureComponent* pOther);

    template <typename T>
    void bindStartCB(void (T::*func)(FixtureComponent*, int), T* const classPtr)//because C++
    {
        m_startCB = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
    }
    template <typename T>
    void bindEndCB(void (T::*func)(FixtureComponent*, int), T* const classPtr)//because C++
    {
        m_endCB = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
    }
    void setIOPos(int ioPos);

protected:
private:
    std::function<void(FixtureComponent*, int)> m_startCB;//other component, their IO position
    std::function<void(FixtureComponent*, int)> m_endCB;//other component, their IO position

    std::tr1::shared_ptr<b2Shape> m_spShape;
    b2FixtureDef m_fixtureDef;
    b2Fixture* m_pFixture;
    int m_ioPos;
};

#endif // FIXTURECOMPONENT_HPP
