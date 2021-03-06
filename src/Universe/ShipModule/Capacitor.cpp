#include "Capacitor.hpp"

Capacitor::Capacitor(const CapacitorData& rData) : ShipModule(rData)
{
    m_storage = rData.storage;

    m_pEnergyPool->changeMax(m_storage);
    m_hasContributed = true;
}
Capacitor::~Capacitor()
{
    m_pEnergyPool->changeMax(-m_storage);
    m_hasContributed = false;
}
void Capacitor::setHealthStateHook(HealthState newState)
{
    if(m_hasContributed && not functioning())
    {
        m_pEnergyPool->changeMax(-m_storage);
        m_hasContributed = false;
    }
    else if(not m_hasContributed && functioning())
    {
        m_pEnergyPool->changeMax(m_storage);
        m_hasContributed = true;
    }
}
