#ifndef TURRET_HPP
#define TURRET_HPP

#include "ShipModule.hpp"
#include "Weapon.hpp"

struct TurretData;

class Turret : public ShipModule
{
public:
    Turret(const TurretData& rData);
    virtual ~Turret();

    void setWep(std::tr1::shared_ptr<const WeaponData> spWep);
    void removeWep();

    void prePhysUpdate() final;
    void postPhysUpdate() final;
    virtual void setAim(const b2Vec2& rTarget);
    virtual void directive(Directive issue);

protected:
private:
    std::tr1::shared_ptr<Weapon> m_spWep;
    int m_shotsRemain;//shots remaining in this volley

    b2Vec2 m_lastAim;
    bool m_hasWep;
};


struct TurretData : public ShipModuleData
{
    TurretData() :
        ShipModuleData(),
        startEmpty(false)//should we start with no weapon
    {
        baseDecor.texName = "radar/radar_base.png";
        baseDecor.animSheetName = "radar/radar_base.acfg";
    }

    bool startEmpty;
    std::tr1::shared_ptr<const WeaponData> startWep;

    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
    {
        TurretData copy(*this);
        copy.pools = stuff;
        copy.fixComp.pBody = pBody;
        return new Turret(copy);
    }
    virtual ModuleData* clone() const
    {
        return new TurretData(*this);
    }
};

#endif // TURRET_HPP
