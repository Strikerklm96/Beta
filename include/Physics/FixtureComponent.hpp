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
           density(1),
           friction(0.3),
           restitution(0.2),
           isSensor(false)
    {

    }
    b2Vec2 offset;//offset from center of body
    ///COLLISION MASK AND CATEGORY
    b2Body* pBody;//who owns us
    Shape shape;

    b2Vec2 size;//size in world coordinates
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
    float getRotation() const;//RADIANS CCW

    void applyForce();//applies force to center of body(Newtons)
    void applyForceFixture();//applies force at the center of fixture
    void applyTorque(float radiansCCW);//applies torque to body(Newton Meters)

    void bindCollisionCb();

protected:
private:
    std::function<void(FixtureComponent*, unsigned)> m_cbFunction;//other component, their IO position

    std::tr1::shared_ptr<b2Shape> m_spShape;
    b2FixtureDef m_fixtureDef;
    b2Fixture* m_pFixture;
};

#endif // FIXTURECOMPONENT_HPP
