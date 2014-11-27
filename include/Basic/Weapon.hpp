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

    void prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, b2Body* pBody);
    void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, b2Body* pBody);

    virtual void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;
    virtual void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;

protected:
    void damage(b2Fixture* pFix, int damage);
    b2Body* m_pBody;

    int m_damage;
    float m_range;
private:
    QuadComponent m_decor;//the weapon sprite

    Timer m_shotTimer;
    int m_shotsRemain;
    int m_shots;

    Energy m_energy;
    Ballistic m_ballistic;

    Timer m_fireTimer;
    float m_fireDelay;
    bool m_shotThisTick;
};


struct WeaponData
{
    WeaponData() :
        ener(3),
        ball(0),

        shots(5),
        damage(50),

        shotDelay(0.09),
        fireDelay(1.5f),

        range(45)
    {
        weaponQuad.animSheetName = "weapons/laser1.acfg";
        weaponQuad.texName = "weapons/laser1.png";
        weaponQuad.layer = GraphicsLayer::ShipAppendagesUpper;
        weaponQuad.dimensions = sf::Vector2f(128, 260);
        weaponQuad.permanentRot = -90.f;
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
