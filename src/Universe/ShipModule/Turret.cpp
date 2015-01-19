#include "Turret.hpp"

#include "Controller.hpp"

Turret::Turret(const TurretData& rData) : ShipModule(rData)
{
    if(not rData.startEmpty)
        setWep(rData.startWep);
}
Turret::~Turret()
{

}
void Turret::prePhysUpdate()
{
    m_spWep->prePhysUpdate(m_fix.getCenter(), m_lastAim, m_fix.getBodyPtr());
    ShipModule::prePhysUpdate();
}
void Turret::postPhysUpdate()
{
    m_spWep->postPhysUpdate(m_fix.getCenter(), m_lastAim, m_fix.getBodyPtr());
    ShipModule::postPhysUpdate();
}
void Turret::directive(Directive issue)
{
    switch(issue)
    {
    case Directive::FirePrimary:
        if(m_spWep)//if we have a weapon
            if(m_spWep->fire(m_pEnergyPool, m_pBallisticPool))
            {

            }
        break;
    default:
        break;
    }
}
void Turret::setWep(std::tr1::shared_ptr<const WeaponData> spWep)
{
    m_spWep.reset(spWep->generate());
}
void Turret::removeWep()
{
    m_spWep.reset();
}
void Turret::setAim(const b2Vec2& rTarget)
{
    m_lastAim = rTarget;
}
