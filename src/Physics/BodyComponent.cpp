#include "BodyComponent.hpp"

#include "Globals.hpp"
#include "Universe.hpp"

BodyComponent::BodyComponent(const BodyComponentData& rData) : m_nw(rData.nwComp, &BodyComponent::pack, &BodyComponent::unpack, this, game.getNwBoss().getNWFactory())
{
    if(rData.isDynamic)
        m_bodyDef.type = b2BodyType::b2_dynamicBody;
    else
        m_bodyDef.type = b2BodyType::b2_staticBody;

    m_bodyDef.position = rData.coords;
    m_bodyDef.bullet = rData.isBullet;
    m_bodyDef.angularDamping = rData.angularDampening;
    m_bodyDef.linearDamping = rData.linearDampening;

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
void BodyComponent::pack(sf::Packet& rPacket)
{
    rPacket << m_pBody->GetPosition().x;
    rPacket << m_pBody->GetPosition().y;
    rPacket << m_pBody->GetLinearVelocity().x;
    rPacket << m_pBody->GetLinearVelocity().y;
    rPacket << m_pBody->GetAngle();
    rPacket << m_pBody->GetAngularVelocity();
}
void BodyComponent::unpack(sf::Packet& rPacket)
{
    b2Vec2 pos;
    b2Vec2 vel;
    float32 angle;
    float32 angleVel;

    rPacket >> pos.x;
    rPacket >> pos.x;
    rPacket >> vel.x;
    rPacket >> vel.y;
    rPacket >> angle;
    rPacket >> angleVel;

    m_pBody->SetTransform(pos, angle);
    m_pBody->SetLinearVelocity(vel);
    m_pBody->SetAngularVelocity(angleVel);
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

