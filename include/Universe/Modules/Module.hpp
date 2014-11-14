#ifndef MODULE_HPP
#define MODULE_HPP

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"
#include "Pool.hpp"

enum class Directive;
struct ModuleData;

class Module
{
public:
    Module(const ModuleData& rData);
    virtual ~Module();

    virtual void prePhysUpdate() = 0;
    virtual void postPhysUpdate();
    virtual void directive(Directive issue);


protected:
    virtual void startContactCB(FixtureComponent* pOther);
    virtual void endContactCB(FixtureComponent* pOther);

    IOComponent m_io;
    NetworkComponent m_nw;
    FixtureComponent m_fix;

    Pool<Missiles>* m_pMissilePool;
    Pool<Ballistic>* m_pBallisticPool;
    Pool<Energy>* m_pEnergyPool;
    Pool<float>* m_pZoomPool;

private:
};


struct ModuleData
{
    ModuleData() :
    //type(ModuleType::Broken),
    ioComp(game.getUniverse().getUniverseIO()),
    nwComp(),
    fixComp()
    {

    }

    IOComponentData ioComp;
    NetworkComponentData nwComp;
    FixtureComponentData fixComp;
    PoolCollection pools;

    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const = 0;
};


#endif // MODULE_HPP
