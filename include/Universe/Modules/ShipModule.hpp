#ifndef SHIPMODULE_HPP
#define SHIPMODULE_HPP

#include "Module.hpp"
#include "QuadComponent.hpp"

struct ShipModuleData;

class ShipModule : public Module
{
public:
    ShipModule(const ShipModuleData& rData);
    virtual ~ShipModule();

    virtual void update(float dT);

protected:
private:
    QuadComponent m_baseDecor;
};



struct ShipModuleData : public ModuleData
{
    ShipModuleData() :
        baseDecor()
        {
        }

        QuadComponentData baseDecor;

    virtual Module* generate(b2Body* pBody) const
    {
        ShipModuleData copy(*this);
        copy.fixComp.pBody = pBody;
        return new ShipModule(copy);
    }
};



#endif // SHIPMODULE_HPP
