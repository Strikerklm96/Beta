#ifndef RADAR_HPP
#define RADAR_HPP

#include "ShipModule.hpp"

struct RadarData;

class Radar : public ShipModule
{
public:
    Radar(const RadarData& rData);
    virtual ~Radar();

    virtual void setHealthStateHook(HealthState newState);

protected:
private:
    bool m_hasContributed;//
    float m_zoom;//zoom change
};


struct RadarData : public ShipModuleData
{
    RadarData() :
        ShipModuleData(),
        zoomAddition(1)// J/s
    {
        baseDecor.texName = "radar/radar_base.png";
        baseDecor.animSheetName = "radar/radar_base.acfg";
    }

    float zoomAddition;//how much more we can zoom with this module active

    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
    {
        RadarData copy(*this);
        copy.pools = stuff;
        copy.fixComp.pBody = pBody;
        return new Radar(copy);
    }
    virtual ModuleData* clone() const
    {
        return new RadarData(*this);
    }
};


#endif // RADAR_HPP
