#include "ProjectileMan.hpp"

ProjectileMan::ProjectileMan()
{

}
ProjectileMan::~ProjectileMan()
{

}
Chunk* ProjectileMan::getProjectile(const std::string& rType)
{
    /**check to make sure we won't crash**/
    /*
    if(m_projList.size()-1 < type)
    {
        ///ERROR LOG
        cout << "\nRequested [" << rType << "] but max was [" << m_projList.size()-1 << "].";
    }**/


    /**check if we have one available by comparing the free index to the size of our projectile list**/
   /* if(get<spList>(m_projList[type]).size() == get<freeIndex>(m_projList[type]))  if not, create AT LEAST one
    {
        add(type); ///how many should we create if we start running out???
    }

    get<freeIndex>(m_projList[type]) += 1; either way, increase free index by 1, and give a pointer to this one

    return (get<spList>(m_projList[type])[get<freeIndex>(m_projList[type])-1].get());**/
}
