#ifndef BALLISTICWEAPON_HPP
#define BALLISTICWEAPON_HPP

#include "Weapon.hpp"
struct BallisticWeaponData;

class BallisticWeapon : public Weapon
{
public:
    BallisticWeapon(const BallisticWeaponData& rData);
    virtual ~BallisticWeapon();

    void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW);
    void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW);
protected:

    float m_velocity;//velocity of projectiles
private:
};



struct BallisticWeaponData : public WeaponData
{
    BallisticWeaponData() :
        velocity(50)
    {
        weaponQuad.animSheetName = "weapons/ballistic1.acfg";
        weaponQuad.texName = "weapons/ballistic1.png";
        ener = 1;///SHOULD BE CONSUMING BALLISTIC, JUST FOR TESTING
        ball = 0;
        mis = 0;
        damage = 500;
        shotDelay = 0.3;
        shots = 1;
        fireDelay = 1;
        range = 100;
    }

    float velocity;//m/s (blocks per second)
    ///SHOULD WE CHOOSE WHAT THE PROJECTILE LOOKS LIKE?

    virtual Weapon* generate() const
    {
        return new BallisticWeapon(*this);
    }
    virtual BallisticWeaponData* clone() const
    {
        return new BallisticWeaponData(*this);
    }
};

#endif // BALLISTICWEAPON_HPP
