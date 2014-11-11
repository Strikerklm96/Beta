#ifndef MODULE_HPP
#define MODULE_HPP

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"

struct ModuleData;

class Module
{
public:
    Module(const ModuleData& rData);
    virtual ~Module();

    virtual void prePhysUpdate() = 0;


protected:
    virtual void startContactCB(FixtureComponent* pOther, int ioPos);
    virtual void endContactCB(FixtureComponent* pOther, int ioPos);

    IOComponent m_io;
    NetworkComponent m_nw;
    FixtureComponent m_fix;

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

    //ModuleType type;
    IOComponentData ioComp;
    NetworkComponentData nwComp;
    FixtureComponentData fixComp;

    virtual Module* generate(b2Body* pBody) const = 0;
};


#endif // MODULE_HPP
