#include "LaserTurret.hpp"
#include "Convert.hpp"
#include "Intelligence.hpp"
#include "IOManager.hpp"

using namespace std;

LaserTurret::LaserTurret(const LaserTurretData& rData) : ShipModule(rData), m_beam(rData.beam)
{
    m_refire.setCountDown(rData.refireTime);
    m_pulseTimer.setCountDown(rData.pulseTime);
    m_showTime = rData.showTime;
    m_fireThisTick = false;
    m_lastAim = b2Vec2(0,0);
    m_range = rData.range;
    m_energyConsumption = rData.energyConsumption;
    m_damage = rData.damage;
    m_shots = rData.shots;
    m_shotsRemain = 0;
    m_ray.setIgnoreBody(m_fix.getBodyPtr());
}
LaserTurret::~LaserTurret()
{

}
void LaserTurret::prePhysUpdate()
{
    if(m_shotsRemain > 0 && m_pulseTimer.isTimeUp())
    {
        m_pulseTimer.restartCountDown();
        b2Vec2 center = m_fix.getCenter();
        float mult = m_range/leon::Dist(m_lastAim, m_fix.getCenter());
        b2Vec2 end = b2Vec2(center.x+(m_lastAim.x-center.x)*mult, center.y+(m_lastAim.y-center.y)*mult);
        game.getUniverse().getWorld().RayCast(&m_ray, m_fix.getCenter(), end);
        m_fireThisTick = true;
        --m_shotsRemain;
    }
    ShipModule::prePhysUpdate();
}
void LaserTurret::postPhysUpdate()
{
    if(m_fireThisTick)
    {
        RayData data = m_ray.getLatest();
        m_ray.reset();
        b2Vec2 center = m_fix.getCenter();
        b2Vec2 end;

        if(data.fixture != NULL)
        {
            end = data.point;
            FixtureComponent& rComp = *static_cast<FixtureComponent*>(data.fixture->GetUserData());
            rComp.getIOPos();
            sf::Packet packet;
            packet << (m_damage/m_shots);

            Message damage;
            damage.reset(rComp.getIOPos(), "damage", packet, 0.f, false);
            game.getUniverse().getUniverseIO().recieve(damage);
        }
        else
        {
            float mult = m_range/leon::Dist(m_lastAim, m_fix.getCenter());
            end = b2Vec2(center.x+(m_lastAim.x-center.x)*mult, center.y+(m_lastAim.y-center.y)*mult);
        }

        m_beam.setStart(m_fix.getCenter());
        m_beam.setEnd(end);
        m_beam.activate(m_showTime, 32, sf::Color::White);
    }
    m_fireThisTick = false;

    ShipModule::postPhysUpdate();
}
void LaserTurret::setAim(const b2Vec2& rTarget)
{
    m_lastAim = rTarget;
}
void LaserTurret::directive(Directive issue)
{
    switch(issue)
    {
    case Directive::FirePrimary:
        if(m_refire.isTimeUp() && m_pEnergyPool->getValue()>m_energyConsumption)
        {
            m_pEnergyPool->changeValue(-m_energyConsumption);
            m_shotsRemain = m_shots;
            m_refire.restartCountDown();
        }
                break;
            default:
                    break;
        }
}
