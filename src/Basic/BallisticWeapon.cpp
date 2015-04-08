#include "BallisticWeapon.hpp"

using namespace std;

BallisticWeapon::BallisticWeapon(const BallisticWeaponData& rData) : Weapon(rData)
{
    m_velocity = rData.velocity;
}
BallisticWeapon::~BallisticWeapon()
{

}
void BallisticWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{
    ///FIRE PROJECTILE
    ///HOW DOES THIS DEAL WITH MULTIPLAYER????
    cout << "\nPreshot";
}
void BallisticWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{
    cout << "\nPostshot";
}
