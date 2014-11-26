#include "ShipModule.hpp"

using namespace std;

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_baseDecor(rData.baseDecor), m_health(rData.health)
{
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
void ShipModule::input(std::string rCommand, sf::Packet rData)
{
    if(rCommand == "damage")
    {
        int val;
        rData >> val;
        m_health.damage(val);
        m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
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
