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

    virtual void prePhysUpdate();
    virtual void postPhysUpdate();
    virtual void setAim(const b2Vec2& rTarget);
    virtual void directive(Directive issue);

protected:
private:
    Beam m_beam;
    RayCastCallback m_ray;

    Timer m_refire;
    float m_showTime;
    Timer m_pulseTimer;

    b2Vec2 m_lastAim;
    float m_range;
    Energy m_energyConsumption;
    int m_damage;//total
    int m_shots;
    int m_shotsRemain;
    bool m_fireThisTick;
};


struct LaserTurretData : public ShipModuleData
{
    LaserTurretData() :
        ShipModuleData(),
        refireTime(1.f),
        pulseTime(0.09),
        showTime(0.05),
        range(20),
        energyConsumption(8),
        damage(10),
        shots(5)
    {
        baseDecor.texName = "turret/laserturret_base.png";
        baseDecor.animSheetName = "turret/laserturret_base.acfg";
    }

    BeamData beam;

    float refireTime;
    float pulseTime;//time between weapon pulses
    float showTime;
    float range;
    Energy energyConsumption;
    int damage;
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
