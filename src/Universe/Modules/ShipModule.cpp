#include "ShipModule.hpp"

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_baseDecor(rData.baseDecor)
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
