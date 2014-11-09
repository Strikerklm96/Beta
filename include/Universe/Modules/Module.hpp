#ifndef MODULE_HPP
#define MODULE_HPP

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"

struct ModuleData
{
    ModuleData() :
    ioComp(game.getUniverse().getUniverseIO()),
    nwComp(),
    fixComp()
    {

    }

    IOComponentData ioComp;
    NetworkComponentData nwComp;
    FixtureComponentData fixComp;

};


class Module
{
public:
    Module(const ModuleData& rData);
    virtual ~Module();

protected:
    IOComponent m_io;
    NetworkComponent m_nw;
    FixtureComponent m_fix;

private:
};

#endif // MODULE_HPP
