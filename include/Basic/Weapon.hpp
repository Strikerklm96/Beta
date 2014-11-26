#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "Pool.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"

struct WeaponData;

class Weapon
{
public:
    Weapon(const WeaponData& rData);
    virtual ~Weapon();

    bool fire(Pool<Energy>* pEnergy, Pool<Ballistic>* pBall);//returns true if we fire

    void prePhysUpdate(const b2Vec2& center, const b2Vec2& aim);
    void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim);

    virtual void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;
    virtual void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;

protected:
    Timer m_shotTimer;
    int m_shotsRemain;

    int m_damage;
    int m_shots;
    float m_range;
private:
    QuadComponent m_decor;//the weapon sprite

    Energy m_energy;
    Ballistic m_ballistic;

    Timer m_fireTimer;
    bool m_shotThisTick;
};


struct WeaponData
{
    WeaponData() :
        ener(8),
        ball(0),

        shots(5),
        damage(25),

        shotDelay(0.09),
        fireDelay(1.f),

        range(25)
    {

    }
    Energy ener;//energy consumed per fire
    Ballistic ball;//ballistics consumed per fire

    int shots;//how many shots per fire
    int damage;//damage we do total

    float shotDelay;//how much time between shots
    float fireDelay;//reload time

    float range;//how far can this shoot

    QuadComponentData weaponQuad;


    virtual Weapon* generate() const
    {
        WeaponData copy(*this);
        std::cout << "\n" << FILELINE;
    }
    virtual WeaponData* clone() const
    {
        std::cout << "\n" << FILELINE;
        return new WeaponData(*this);
    }
};

#endif // WEAPON_HPP
