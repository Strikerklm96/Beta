#include "Projectile.hpp"

Projectile::Projectile(const ProjectileData& rData) : m_body(rData.body), m_module(rData.module)
{

}
Projectile::~Projectile()
{

}
