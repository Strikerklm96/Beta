#include "GameObject.hpp"

using namespace std;

GameObject::GameObject(const GameObjectData& rData) : m_io(rData.ioComp, &GameObject::input, this), m_nw(rData.nwComp, &GameObject::pack, &GameObject::unpack, this, game.getNwBoss().getNWFactory())
{

}
GameObject::~GameObject()
{

}
void GameObject::input(std::string rCommand, sf::Packet rData)
{
    cout << "\nCommand not found in [" << m_io.getName() << "].";
}
void GameObject::pack(sf::Packet& rPacket)
{
    int32_t bytes = 0;
    rPacket << bytes;
    cout << "\nB:" << bytes;
}
void GameObject::unpack(sf::Packet& rPacket)
{
    int32_t size;
    rPacket >> size;
    cout << "\n0," << size << "gameObject";
}
