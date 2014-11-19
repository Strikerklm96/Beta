#include "Capacitor.hpp"

Capacitor::Capacitor(const CapacitorData& rData) : ShipModule(rData)
{
    m_storage = rData.storage;

    m_pEnergyPool->changeMax(m_storage);
}
Capacitor::~Capacitor()
{
    m_pEnergyPool->changeMax(-m_storage);
}
