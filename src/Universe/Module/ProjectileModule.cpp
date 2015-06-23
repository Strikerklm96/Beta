#include "ProjectileModule.hpp"

ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData), m_decor(rData.baseDecor)
{

}
ProjectileModule::~ProjectileModule()
{

}
void ProjectileModule::postPhysUpdate()
{
	m_decor.setPosition(m_fix.getCenter());
	m_decor.setRotation(m_fix.getAngle());
}
