#ifndef PROJECTILEMODULE_HPP
#define PROJECTILEMODULE_HPP

#include "ShipModule.hpp"

struct ProjectileModuleData;

class ProjectileModule : public ShipModule
{
public:
    ProjectileModule(const ProjectileModuleData& rData);
    virtual ~ProjectileModule();

    virtual void prePhysUpdate();

protected:
private:
};


struct ProjectileModuleData : public ShipModuleData
{
    ProjectileModuleData() :
        ShipModuleData()
    {
        fixComp.isSensor = true;
        fixComp.shape = Shape::Circle;
        fixComp.colCategory = Category::Projectile;
        fixComp.colMask = Mask::Projectile;
        baseDecor.animSheetName = "projectile/ballistic1.acfg";
        baseDecor.texName = "projectile/ballistic1.png";
        baseDecor.layer = GraphicsLayer::Projectiles;
    }


    virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
    {
        ProjectileModuleData copy(*this);
        copy.pools = stuff;
        copy.fixComp.pBody = pBody;
        return new ProjectileModule(copy);
    }
    virtual ModuleData* clone() const
    {
        return new ProjectileModuleData(*this);
    }
};

#endif // PROJECTILEMODULE_HPP
