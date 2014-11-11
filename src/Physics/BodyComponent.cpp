#include "BodyComponent.hpp"

#include "Globals.hpp"
#include "Universe.hpp"

BodyComponent::BodyComponent(const BodyComponentData& rData)
{
    m_bodyDef.bullet = rData.isBullet;
    if(rData.isDynamic)
        m_bodyDef.type = b2BodyType::b2_dynamicBody;
    else
        m_bodyDef.type = b2BodyType::b2_staticBody;
    m_bodyDef.position = rData.coords;

    m_pBody = game.getUniverse().getWorld().CreateBody(&m_bodyDef);
    m_pBody->SetUserData(this);

    m_awake = true;//regardless, set us to be awake
    if(!rData.startAwake)//if it should be asleep
        sleep();//then cleanly put it to sleep
}
BodyComponent::~BodyComponent()
{

}
const b2Vec2& BodyComponent::getPosition() const
{
    return m_pBody->GetPosition();
}
b2Body* BodyComponent::getBodyPtr()
{
    return m_pBody;
}


bool BodyComponent::isAwake() const
{
    return m_awake;
}
void BodyComponent::sleep()
{
///TODO
}
void BodyComponent::wake()
{
///TODO
}
void BodyComponent::wake(b2Vec2 coords, float radiansCCW, b2Vec2 velocity, float angularVel)
{
///TODO
}

