#include "Module.hpp"

#include "Intelligence.hpp"

using namespace std;

Module::Module(const ModuleData& rData) : m_io(rData.ioComp), m_nw(rData.nwComp), m_fix(rData.fixComp)
{
    m_fix.setIOPos(m_io.getPosition());
    m_fix.bindStartCB(Module::startContactCB, this);
    m_fix.bindEndCB(Module::endContactCB, this);

    m_pMissilePool = rData.pools.missilePool;
    m_pEnergyPool = rData.pools.energyPool;
    m_pZoomPool = rData.pools.zoomPool;
    m_pBallisticPool = rData.pools.ballisticPool;
}
Module::~Module()
{

}
void Module::postPhysUpdate()
{

}
void Module::directive(Directive issue)
{

}
void Module::startContactCB(FixtureComponent* pOther)
{

}
void Module::endContactCB(FixtureComponent* pOther)
{

}
