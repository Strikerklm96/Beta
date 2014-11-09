#include "Module.hpp"

Module::Module(const ModuleData& rData) : m_io(rData.ioComp), m_nw(rData.nwComp), m_fix(rData.fixComp)
{

}
Module::~Module()
{

}
