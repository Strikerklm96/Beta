#include "ProjectileMan.hpp"

ProjectileMan::ProjectileMan()
{

}
ProjectileMan::~ProjectileMan()
{

}
Projectile* ProjectileMan::getProjectile(const std::string& rBPName)
{
	/**FIND LIST**/
	auto it = m_projectileList.find(rBPName);
	if(it != m_projectileList.cend())
	{
	    int index = it->second.first;
	    std::vector<sptr<Projectile> >& rList = it->second.first;
	    ++(it->second.first);
	    /**FIND PROJECTILE IN LIST**/
        if(index != rList.size())
            return rList[index];
        else
        {
            addNew(rBPName);
        }
	}
	else
    {
        ///ERROR LOG
        cout << FILELINE;
		return NULL;
    }
}
