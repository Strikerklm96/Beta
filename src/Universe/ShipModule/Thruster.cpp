#include "Thruster.hpp"
#include "Controller.hpp"

using namespace std;

Thruster::Thruster(const ThrusterData& rData) : ShipModule(rData)
{
    m_eConsump = rData.energyConsumption;
    m_force = rData.force;
    m_torque = rData.torque;

    m_forceVec = b2Vec2(0,1);
    m_isCCW = true;

    m_tickForce = false;
    m_tickTorque = false;
}
Thruster::~Thruster()
{

}
void Thruster::prePhysUpdate()
{
    ShipModule::prePhysUpdate();
    if(m_tickForce)
    {
        thrust(m_forceVec);
    }
    if(m_tickTorque)
    {
        torque(m_isCCW);
    }
}
void Thruster::postPhysUpdate()
{
    ShipModule::postPhysUpdate();
    m_tickForce = false;
    m_tickTorque = false;
}
void Thruster::directive(Directive issue)
{
    switch(issue)
    {
    case Directive::Up:
        m_tickForce = true;
        m_forceVec = b2Vec2(0,1);
        break;
    case Directive::Down:
        m_tickForce = true;
        m_forceVec = b2Vec2(0,-1);
        break;
    case Directive::RollCCW:
        m_tickTorque = true;
        m_isCCW = true;
        break;
    case Directive::RollCW:
        m_tickTorque = true;
        m_isCCW = false;
        break;
    default:
        break;
    }
}
void Thruster::thrust(const b2Vec2& rDirection)
{
    float eThisStep = m_eConsump*game.getUniverse().getTimeStep();

    if(eThisStep <= m_pEnergyPool->getValue())
    {
        m_pEnergyPool->changeValue(-eThisStep);

        float angle = m_fix.getAngle();

        b2Vec2 forceVec;
        forceVec.x = cos(-angle)*rDirection.x + sin(-angle)*rDirection.y;//negative because THAT IS CORRECT, go lookup the equation!
        forceVec.y = -sin(-angle)*rDirection.x + cos(-angle)*rDirection.y;

        forceVec.x *= m_force;
        forceVec.y *= m_force;

        m_fix.applyForce(forceVec);
    }
}
void Thruster::torque(bool CCW)
{
    float eThisStep = m_eConsump*game.getUniverse().getTimeStep();

    if(eThisStep <= m_pEnergyPool->getValue())
    {
        m_pEnergyPool->changeValue(-eThisStep);
        if(CCW)
            m_fix.applyTorque(m_torque);
        else
            m_fix.applyTorque(-m_torque);
    }
}
