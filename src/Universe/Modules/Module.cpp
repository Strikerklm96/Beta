#include "Module.hpp"

using namespace std;

Module::Module(const ModuleData& rData) : m_io(rData.ioComp), m_nw(rData.nwComp), m_fix(rData.fixComp)
{
    m_fix.setIOPos(m_io.getPosition());
    m_fix.bindStartCB(Module::startContactCB, this);
    m_fix.bindEndCB(Module::endContactCB, this);
}
Module::~Module()
{

}

void Module::startContactCB(FixtureComponent* pOther, int ioPos)
{

}
void Module::endContactCB(FixtureComponent* pOther, int ioPos)
{

}
