#ifndef REACTOR_HPP
#define REACTOR_HPP

#include "ShipModule.hpp"

struct ReactorData;

class Reactor : public ShipModule
{
public:
    Reactor(const ReactorData& rData);
    virtual ~Reactor();

    virtual void prePhysUpdate();
    virtual void postPhysUpdate();

protected:
private:
    float m_rate;//energy per second
};


struct ReactorData : public ShipModuleData
{
    ReactorData() :
        ShipModuleData(),
        rate(5)// J/s
    {
        baseDecor.texName = "reactor/reactor_base.png";
        baseDecor.animSheetName = "reactor/reactor_base.acfg";
    }

    float rate;// J/s

    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
    {
        ReactorData copy(*this);
        copy.pools = stuff;
        copy.fixComp.pBody = pBody;
        return new Reactor(copy);
    }
};


#endif // REACTOR_HPP
