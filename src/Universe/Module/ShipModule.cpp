#include "ShipModule.hpp"

using namespace std;

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_baseDecor(rData.baseDecor), m_health(rData.health)
{
    m_healthState = rData.initHealthState;
    m_functionsDamaged = rData.functionsDamaged;
    m_baseDecor.setPosition(m_fix.getCenter());
    m_baseDecor.setRotation(m_fix.getAngle());
}
ShipModule::~ShipModule()
{

}
void ShipModule::prePhysUpdate()
{

}
void ShipModule::postPhysUpdate()
{
    m_baseDecor.setPosition(m_fix.getCenter());
    m_baseDecor.setRotation(m_fix.getAngle());
}

void ShipModule::pack(sf::Packet& rPacket)
{
    int32_t bytes = 0;
    rPacket << bytes;
    cout << "\nB:" << bytes;
}
void ShipModule::unpack(sf::Packet& rPacket)
{
    int32_t size;
    rPacket >> size;
    cout << "\n0," << size << "shipmod";
}
void ShipModule::input(std::string rCommand, sf::Packet rData)
{
    if(rCommand == "damage")
    {
        int val;
        rData >> val;
        m_health.damage(val);
        m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
        if(m_health.getHealth() <= 0)
            setHealthState(HealthState::Broken);

        cout << "\n" << m_health.getHealth();
    }
    else if(rCommand == "heal")
    {
        int val;
        rData >> val;
        m_health.heal(val);
        m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
    }
    else
        Module::input(rCommand, rData);
}
bool ShipModule::functioning()//does this module still do its function
{
    if(m_healthState == HealthState::Nominal)
        return true;
    else if(m_healthState == HealthState::Broken)
        return false;
    else if(m_healthState == HealthState::Damaged)
    {
        if(m_functionsDamaged)
            return true;
        else
            return false;
    }
    cout << "\n" << FILELINE;
}
void ShipModule::setHealthState(HealthState newState)
{
    m_healthState = newState;
    ///MODIFY COLLISION CATEGORY so this module can be shot past!
    setHealthStateHook(newState);
}
void ShipModule::setHealthStateHook(HealthState newState)
{

}
