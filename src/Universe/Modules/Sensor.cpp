#include "Sensor.hpp"

using namespace std;

Sensor::Sensor(const SensorData& rData) : Module(rData)
{

}
Sensor::~Sensor()
{

}
void Sensor::prePhysUpdate()
{

}
void Sensor::startContactCB(FixtureComponent* pOther)
{
    m_guests.push_back(pOther);
}
void Sensor::endContactCB(FixtureComponent* pOther)
{
    for(auto it=m_guests.cbegin(); it!=m_guests.cend(); ++it)
        if((*it) == pOther)
        {
            m_guests.erase(it);
            break;
        }
}
