#include "LaserTurret.hpp"
#include "Convert.hpp"

using namespace std;

LaserTurret::LaserTurret(const LaserTurretData& rData) : ShipModule(rData), m_beam(rData.beam)
{
    m_lastAim = b2Vec2(0,0);
    m_range = rData.range;
    m_energyConsumption = rData.energyConsumption;
    m_damage = rData.damage;
    m_shots = rData.shots;
    m_ray.setIgnoreBody(m_fix.getBodyPtr());
}
LaserTurret::~LaserTurret()
{

}
void LaserTurret::postPhysUpdate()
{
    RayData data = m_ray.getLatest();
    m_ray.reset();
    b2Vec2 center = m_fix.getCenter();
    b2Vec2 end;

    if(data.fixture != NULL)
    {
        end = data.point;
        FixtureComponent& rComp = *static_cast<FixtureComponent*>(data.fixture->GetUserData());
        b2Vec2 dir((m_lastAim.x-center.x), (m_lastAim.y-center.y));
        dir.Normalize();
        dir.x *=4;
        dir.x *=4;

        rComp.applyForceFixture(dir);
    }
    else
    {
        float mult = m_range/leon::Dist(m_lastAim, m_fix.getCenter());
        end = b2Vec2(center.x+(m_lastAim.x-center.x)*mult, center.y+(m_lastAim.y-center.y)*mult);
    }

    m_beam.setStart(m_fix.getCenter());
    m_beam.setEnd(end);
    m_beam.activate(60, 32, sf::Color::White);

    ShipModule::postPhysUpdate();
}
void LaserTurret::setAim(const b2Vec2& rTarget)
{
    m_lastAim = rTarget;
    b2Vec2 center = m_fix.getCenter();
    float mult = m_range/leon::Dist(m_lastAim, m_fix.getCenter());
    b2Vec2 end = b2Vec2(center.x+(m_lastAim.x-center.x)*mult, center.y+(m_lastAim.y-center.y)*mult);
    game.getUniverse().getWorld().RayCast(&m_ray, m_fix.getCenter(), end);
}
