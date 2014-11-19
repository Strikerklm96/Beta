#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "Module.hpp"

struct SensorData;

class Sensor : public Module
{
public:
    Sensor(const SensorData& rData);
    virtual ~Sensor();

    virtual void prePhysUpdate();

    virtual void entered(FixtureComponent* pOther) = 0;
    virtual void exited(FixtureComponent* pOther) = 0;

protected:
    void startContactCB(FixtureComponent* pOther) final;
    void endContactCB(FixtureComponent* pOther) final;

    std::vector<FixtureComponent*> m_guests;
private:

};



struct SensorData : public ModuleData
{
    SensorData()
    {
        fixComp.isSensor = true;
        fixComp.density = 0.f;
    }


    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const = 0;
};



#endif // SENSOR_HPP
