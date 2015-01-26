#include "Weapon.hpp"
#include "FixtureComponent.hpp"
#include "Message.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"

using namespace std;

Weapon::Weapon(const WeaponData& rData) : m_decor(rData.weaponQuad)
{
    m_energy = rData.ener;
    m_ballistic = rData.ball;

    m_range = rData.range;
    m_damage = rData.damage;
    m_shots = rData.shots;
    m_shotsRemain = 0;

    m_fireTimer.setCountDown(rData.fireDelay);
    m_fireTimer.restartCountDown();
    m_fireDelay = rData.fireDelay;
    m_shotTimer.setCountDown(rData.shotDelay);
    m_shotTimer.restartCountDown();

    m_shotThisTick = false;
}
Weapon::~Weapon()
{

}
bool Weapon::fire(Pool<Energy>* pEnergy, Pool<Ballistic>* pBall)
{

    if(m_fireTimer.isTimeUp() && pEnergy->getValue()>=m_energy && pBall->getValue()>=m_ballistic)
    {
        m_decor.getAnimator().setAnimation("Fire", m_fireDelay);
        m_fireTimer.restartCountDown();
        pEnergy->changeValue(-m_energy);
        pBall->changeValue(-m_ballistic);
        m_shotsRemain = m_shots;

        return true;
    }
    else
        return false;
}
void Weapon::prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 angle, b2Body* pBody)
{
    m_pBody = pBody;

    if(m_shotsRemain>0 && m_shotTimer.isTimeUp())
    {
        --m_shotsRemain;
        m_shotTimer.restartCountDown();
        m_shotThisTick = true;

        preShot(center, aim, angle);
    }
}
void Weapon::postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 angle, b2Body* pBody)
{
    m_pBody = pBody;

    m_decor.setRotation(angle);
    m_decor.setPosition(center);

    if(m_shotThisTick)
    {
        m_shotThisTick = false;

        postShot(center, aim, angle);
    }
}
void Weapon::damage(b2Fixture* pFix, int damage)
{
    FixtureComponent& rComp = *static_cast<FixtureComponent*>(pFix->GetUserData());
    rComp.getIOPos();
    sf::Packet packet;
    packet << (m_damage/m_shots);
    Message mess;
    mess.reset(rComp.getIOPos(), "damage", packet, 0.f, false);
    game.getUniverse().getUniverseIO().recieve(mess);
}
