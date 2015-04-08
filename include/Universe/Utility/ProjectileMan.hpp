#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP




/**MANAGES PROJECTILES**/
class ProjectileMan
{
public:
    ProjectileMan();
    virtual ~ProjectileMan();


protected:
private:
    std::map<std::string, std::vector<sptr< > > > m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
