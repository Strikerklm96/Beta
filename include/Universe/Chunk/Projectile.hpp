#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "BodyComponent.hpp"
#include "ProjectileModule.hpp"

struct ProjectileData
{
	ProjectileData() :
		body(),
		module()
	{
		body.angularDampening = 0;
		body.coords = b2Vec2(25,25);
		body.isBullet = true;
		body.isDynamic = true;
		body.linearDampening = 0;
		body.rotation = 0;
		body.startAwake = true;
	}

	BodyComponentData body;
	ProjectileModuleData module;
};

class Projectile
{
public:
	Projectile(const ProjectileData& rData);
	virtual ~Projectile();

	void launch(const b2Vec2& rStart, const b2Vec2&Vel, float radCCW, float radCCWps);// coord, linVel, rot, rotVel
	void prep(const Message& rPayload);//set our payload
	void reset();//this projectile will go back into projectile pool

protected:
	BodyComponent m_body;
	ProjectileModule m_module;
private:

};


#endif // PROJECTILE_HPP
