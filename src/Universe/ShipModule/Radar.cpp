#include "Radar.hpp"

Radar::Radar(const RadarData& rData) : ShipModule(rData)
{
    m_zoom = rData.zoomAddition;

    m_pZoomPool->changeValue(m_zoom);
    m_hasContributed = true;
}
Radar::~Radar()
{
    m_pZoomPool->changeValue(-m_zoom);
    m_hasContributed = false;
}
void Radar::setHealthStateHook(HealthState newState)
{
    if(m_hasContributed && not functioning())
    {
        m_pZoomPool->changeValue(-m_zoom);
        m_hasContributed = false;
    }
    else if(not m_hasContributed && functioning())
    {
        m_pZoomPool->changeValue(m_zoom);
        m_hasContributed = true;
    }
}
