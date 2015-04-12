#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
class Chunk;

/**MANAGES PROJECTILES**/
class ProjectileMan
{
public:
    ProjectileMan();
    virtual ~ProjectileMan();

    Chunk* getProjectile(const std::string& rType);

protected:
private:
    std::map<std::string, std::vector<sptr<Chunk> > > m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
