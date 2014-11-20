#include "Radar.hpp"

Radar::Radar(const RadarData& rData) : ShipModule(rData)
{
    m_zoom = rData.zoomAddition;

    m_pZoomPool->changeValue(m_zoom);
}
Radar::~Radar()
{
    m_pZoomPool->changeValue(-m_zoom);
}
