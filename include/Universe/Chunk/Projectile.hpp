#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

enum class ProjectileType
{
    BallisticLarge,
    BallisticSmall,

    MissileLarge,
    MissileSmall,
};

struct ProjectileData
{

};

class Projectile
{
public:
    Projectile(const ProjectileData& rData);
    virtual ~Projectile();

protected:
private:
};

#endif // PROJECTILE_HPP
