#ifndef PLATING_HPP
#define PLATING_HPP

#include "ShipModule.hpp"
#include "Beam.hpp"
#include "RayCastCallback.hpp"

struct LaserTurretData;

class LaserTurret : public ShipModule
{
public:
    LaserTurret(const LaserTurretData& rData);
    virtual ~LaserTurret();

    virtual void postPhysUpdate();
    virtual void setAim(const b2Vec2& rTarget);

protected:
private:
    Beam m_beam;
    RayCastCallback m_ray;

    b2Vec2 m_lastAim;
    float m_range;
    float m_energyConsumption;
    float m_damage;//total
    int m_shots;
};


struct LaserTurretData : public ShipModuleData
{
    LaserTurretData() :
        ShipModuleData(),
        range(20),
        energyConsumption(1),
        damage(10),
        shots(4)
    {
        baseDecor.texName = "turret/laserturret_base.png";
        baseDecor.animSheetName = "turret/laserturret_base.acfg";
    }

    BeamData beam;

    float range;
    float energyConsumption;
    float damage;
    int shots;

    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
    {
        LaserTurretData copy(*this);
        copy.pools = stuff;
        copy.fixComp.pBody = pBody;
        return new LaserTurret(copy);
    }
    virtual ModuleData* clone() const
    {
        return new LaserTurretData(*this);
    }
};


#endif // PLATING_HPP
